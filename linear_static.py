#coding: utf-8 -*-
"""Qt wizard on linear static study case
"""
from aster_s.utils import log_gui
from PyQt4 import QtGui as qt
from PyQt4 import QtCore as qtc
import aster_s.salome_tree as ST
import aster_s.wizards.linear_static as LS
import aster_s.wizards.common as WCD
import aster_s_gui
import aster_s_gui.wizards.common as WC
from aster_s.utils import log_gui
QNULL = WC.QNULL
ROOT_MIDX = WC.ROOT_MIDX
invalided = "--"
SIG = qtc.SIGNAL
import copy

def is_valid_mesh(cexp, mesh, mod):
    """A valid mesh needs to have groups for pressure"""
    is_valid = True
    log_gui.debug("is_valid_mesh %s / %s", mesh, mod)
    if not mesh:
        mess = "A mesh is required"
        mod.launch(aster_s_gui.ERROR, mess)
        is_valid = False
        return is_valid
    if not cexp.give("pressure").find_groups(mesh):
        mess = "At least a group without nodes need to be defined " \
               "on the selected object"
        mod.launch(aster_s_gui.ERROR, mess)
        is_valid = False
    return is_valid


class SMeshExp(WC.CompoExp):
    """Linear static SMESH explorator
    """

    def __init__(self):
        WC.CompoExp.__init__(self)
        SMesh = ST.SMeshExplorator
        exp = SMesh()
        no_grp = exp.add_group(LS.GRP_NO)
        no_grp.register((2, 3), [SMesh.node])
        ma_grp = exp.add_group(LS.GRP_MA)
        ma_grp.register((2, 3), [SMesh.edge])
        ma_grp.register((3,), [SMesh.face, SMesh.volume])
        self.register("boundaries", exp)
        self.register("pressure", ma_grp)

    def validate(self, mesh, mod):
        """A valid mesh needs to have mesh groups for defining pressure"""
        return is_valid_mesh(self, mesh, mod)


class GeomExp(WC.CompoExp):
    """Linear static GEOM explorator
    """

    def __init__(self):
        WC.CompoExp.__init__(self)
        Geom = ST.GeomExplorator
        exp = Geom()
        no_grp = exp.add_group(LS.GRP_NO)
        no_grp.register((2, 3), [Geom.vertex])
        ma_grp = exp.add_group(LS.GRP_MA)
        ma_grp.register((2, 3), [Geom.edge])
        ma_grp.register((3,), [Geom.face, Geom.shell])
        self.register("boundaries", exp)
        self.register("pressure", ma_grp)

    def validate(self, mesh, mod):
        """A valid geometry needs to have mesh groups for defining pressure"""
        return is_valid_mesh(self, mesh, mod)


class PressurePage(WC.WizardPage):
    """Wizard page on pressure loading
    """

    def cleanupPage(self):
        """Clean page in case user navigates"""
        WC.WizardPage.cleanup(self)

    def initializePage(self):
        """Query the model on meshes"""
        WC.WizardPage.initialize(self)
        self.page.use(qt.QVBoxLayout())
        exp = self.give_field("exp-store").give_exp("pressure")
        grps = exp.find_groups(self.give_field("mesh"))
        dims = [(u"Pressure", 1.)]
        tit = u"Adding pressure on meshes groups"
        # The last groups should be seen first
        grps.reverse()
        WC.add_condition_selector(self, grps, dims, "pressure-loading*", tit)


def add_pressure_page(wiz):
    """Add page on pressure loading"""
    page = wiz.add_page(u"Boundaries conditions", PressurePage())
    page.register("pressure-loading*", None)


class FinalPage(WC.FinalPage):
    """Build case
    """

    def validatePage(self):
        """Validate the wizard"""
        getf = self.give_field

        comm = LS.CommWriter()
        comm.use(LS.Modelisation(getf("model")))
        comm.use(LS.YoungModulus(self.get_float("young-modulus")))
        comm.use(LS.PoissonRatio(self.get_float("poisson-ratio")))
        mech_consts = comm.use(LS.MechConstraints())
        bound_conds = mech_consts.add(LS.BoundConds())
        mesh = getf("mesh")
        exp = getf("exp-store").give_exp("boundaries")

        if not getf("pressure-loading") and not getf("group-boundaries"):
            mess = "Pressure and/or boundary conditions are required "
            self._mod.launch(aster_s_gui.ERROR, mess)
            return False
        else :
            if getf("group-boundaries"):
                for cond in getf("group-boundaries"):
                    gname = str(cond[0])
                    grp_type = exp.give_group_key(mesh, gname)
                    bound_conds.add(LS.DplFromName(grp_type, gname, *cond[1:]))
            else:
                mess = "Warning: You have not defined boundary conditions "
                self._mod.launch(aster_s_gui.INFO, mess)
            pressure = mech_consts.add(LS.Pressure())
            if getf("pressure-loading"):
                for gname, val in getf("pressure-loading"):
                    pressure.add(LS.GrpPres(str(gname), val))
            else:
                mess = "Warning: You have not defined pressure "
                self._mod.launch(aster_s_gui.INFO, mess)
            comm.write(self.get_str("command-file"))
            self.add_case("linear-static")
            return True
            
def Creat_Mesh_Select_DockWdg(mod):
   desktop = mod.give_qtwid()
   if desktop is None:
    mess = "This script needs to be run from the Salome GUI menu " \
           "by using 'File -> Load Script'."
    raise ValueError(mess)
   else:    
    #exp_store = WC.ExpStore()
    #exp_store.register(WC.ExpStore.smesh, SMeshExp())
    #exp_store.register(WC.ExpStore.geom, GeomExp())
    #lay = qt.QVBoxLayout()
    #wdg.setLayout(lay)
    #verticalSpacer = qt.QSpacerItem(20, 40, qt.QSizePolicy.Minimum, qt.QSizePolicy.Expanding)
    #lay.addItem(verticalSpacer)
    #lay.addWidget(qt.QLabel("Select a mesh from the Salome object browser"))
    dockwdg = WC.CustomDockWidget(u"Isotropic linear elastic study", u"Mesh selection", desktop)
    dockwdg.setWindowTitle("Mesh selection")
    wdg = WC.MeshWdg(mod,dockwdg)
    dockwdg.setCenterWidget(wdg)
    desktop.addDockWidget(qtc.Qt.RightDockWidgetArea,dockwdg)
    
def Creat_Model_Definition_DockWdg(desktop):
   if desktop is None:
    mess = "This script needs to be run from the Salome GUI menu " \
           "by using 'File -> Load Script'."
    raise ValueError(mess)
   else:
    wdg = WC.WizardPage()
    lay = qt.QVBoxLayout()
    wdg.setLayout(lay)
    verticalSpacer = qt.QSpacerItem(20, 40, qt.QSizePolicy.Minimum, qt.QSizePolicy.Expanding)
    lay.addItem(verticalSpacer)
    lay.addWidget(qt.QLabel("What kind of model do you want to work on?"))
    wfield = WC.WizardField(wdg,"abc",0)
    lay.addWidget(WC.ModelSelection(wfield, [
        WC.Mode3D,
        WC.PlaneStress,
        WC.PlaneStrain,
        WC.AxisSymmetric,]))
    dockwdg = WC.CustomDockWidget(u"Isotropic linear elastic study", u"Model definition", desktop)
    dockwdg.setWindowTitle(u"Model definition")
    dockwdg.setCenterWidget(wdg)
    desktop.addDockWidget(qtc.Qt.RightDockWidgetArea,dockwdg)

def Creat_Material_Property_DockWdg(desktop):
   if desktop is None:
    mess = "This script needs to be run from the Salome GUI menu " \
           "by using 'File -> Load Script'."
    raise ValueError(mess)
   else:
    dockwdg = WC.CustomDockWidget(u"Isotropic linear elastic study", u"Material properties", desktop)
    dockwdg.setWindowTitle("Material properties")
    wdg = WC.Material_Wdg(dockwdg)
    dockwdg.setCenterWidget(wdg)
    desktop.addDockWidget(qtc.Qt.RightDockWidgetArea,dockwdg)

def Creat_Boundary_Degrees_Conditions_DockWdg(mod):
   desktop = mod.give_qtwid()
   if desktop is None:
    mess = "This script needs to be run from the Salome GUI menu " \
           "by using 'File -> Load Script'."
    raise ValueError(mess)
   else:
    dockwdg = WC.CustomDockWidget(u"Isotropic linear elastic study", u"Boundaries Degrees conditions", desktop)
    dockwdg.setWindowTitle("Boundaries Degrees conditions")
    exp_store = WC.ExpStore()
    exp_store.register(WC.ExpStore.smesh, SMeshExp())
    exp_store.register(WC.ExpStore.geom, GeomExp())
    exp_store.use(WC.ExpStore.geom)
    exp = exp_store.give_exp("boundaries")
    boundarywdg = WC.Bounaries_Conditions_Wdg(mod,exp,"boundaries",dockwdg)
    dockwdg.setCenterWidget(boundarywdg)
    desktop.addDockWidget(qtc.Qt.RightDockWidgetArea,dockwdg)
    
def Creat_Boundary_Pressure_Conditions_DockWdg(mod):
   desktop = mod.give_qtwid()
   if desktop is None:
    mess = "This script needs to be run from the Salome GUI menu " \
           "by using 'File -> Load Script'."
    raise ValueError(mess)
   else:
    dockwdg = WC.CustomDockWidget(u"Isotropic linear elastic study", u"Boundaries Pressure conditions", desktop)
    dockwdg.setWindowTitle("Boundaries Pressure conditions")
    exp_store = WC.ExpStore()
    exp_store.register(WC.ExpStore.smesh, SMeshExp())
    exp_store.register(WC.ExpStore.geom, GeomExp())
    exp_store.use(WC.ExpStore.geom)
    exp = exp_store.give_exp("pressure")
    boundarywdg = WC.Bounaries_Conditions_Wdg(mod,exp,u"pressure",dockwdg)
    dockwdg.setCenterWidget(boundarywdg)
    desktop.addDockWidget(qtc.Qt.RightDockWidgetArea,dockwdg)
SIG = qtc.SIGNAL
connect = qtc.QObject.connect

class LSData(WC.AstData):

    def ini(self):
        self.exp_store.register(WC.ExpStore.smesh, SMeshExp())
        self.exp_store.register(WC.ExpStore.geom, GeomExp())

    def save_nolinear(self):
        """Validate the wizard"""
        comm = LS.AstCommWriter()
        comm.use(LS.Modelisation(self.get_sel_itm().get_model_type()))
        gm = LS.GrpMaterial()
        gm.clear_part()
        grp_material = comm.use(gm)
            
        mech_consts = comm.use(LS.MechConstraints())
        bound_conds = mech_consts.add(LS.BoundConds())

        grp_material.add(LS.Material(self.get_material()))#material
        grp_material.add(LS.GroupMaterial(self.materials_conds))#grouop-material
        #####################BondContact#########
        bond_group=[]
        fric_group=[]
        fricless_group=[]
        grp_pair = self.pair_conds
        for g in grp_pair:
            if(str(g[0])==u"bond"):
               bond_group.append([str(g[1]),str(g[2])])
            if(str(g[0])==u"Friction"):
               fric_group.append([str(g[1]),str(g[2]),str(g[3])])
            if(str(g[0])==u"Frictionless"):
               fricless_group.append([str(g[1]),str(g[2]),"0.0"])
        
        bc=LS.BondContact(bond_group)
        bond_contact = comm.use(bc)
        bc.clear_part()
        
        #################################
        
        if(len(bond_group)):
             comm.use(WCD.HasBondContactKey("_F(CHARGE=CHA3)"))
        else:
             comm.use(WCD.HasBondContactKey(""))
        
        if(len(fric_group+fricless_group)):
             comm.use(WCD.HasfrictionContactKey("CONTACT = CONTA,"))
        else:
             comm.use(WCD.HasfrictionContactKey(""))
        #########################
        
        mc=LS.MechConstraints()
        mc.clear_consts()
        mech_consts = comm.use(mc)#start replace a line
        bound_conds = mech_consts.add(LS.BoundConds())#first sub section
        bound_conds.clear_consts()
        mesh = self.mesh
        exp = self.exp_store.give_exp("boundaries")
        
        log_gui.debug("save_nolinear %d", len(self.degrees_conds))
        if len(self.degrees_conds):
            for cond in self.degrees_conds:
                gname = str(cond[0])
                grp_type = exp.give_group_key(mesh, gname)
                bound_conds.add(LS.DplFromName(grp_type, gname, *cond[1:]))
        else:
            mess = "Warning: You have not defined boundary conditions  "
            self.mod.launch(aster_s_gui.INFO, mess)

        mech_consts.add(LS.BoundEnd(len("CHARC=AFFE_CHAR_MECA"),");"))
        mech_consts.add(LS.PressStart(0,"CHARF=AFFE_CHAR_MECA(MODELE=MODX,"))

        pressure = mech_consts.add(LS.Pressure() )#here
        forceface = mech_consts.add(LS.ForceFace() )#here 
        forcenodale = mech_consts.add(LS.ForceNodale() )#here
        
        for gname, ptype, v1,v2,v3,v4,v5,v6 in self.pressure_conds:
            if ptype =="Pressure":
                pressure.add(LS.GrpPres(str(gname), v1))
            elif ptype =="Force_Face":
                forceface.add(LS.ForceFaceArgs(str(gname), v1,v2,v3))
            elif ptype =="Force_Node":
                 forcenodale.add(LS.ForceNodaleArgs(str(gname), v1,v2))

        groups=set()
        for g in grp_pair:#todo:
            if(mesh.get_mesh_grounp_type(str(g[1])) != "VOLUME"):
                groups.add(str(g[1]))
            if(mesh.get_mesh_grounp_type(str(g[2])) != "VOLUME"):
                groups.add(str(g[2]))

        for g in groups:
            pressure.add_to_mesh_change(g)
            
        ##################
        par=[]
        tmp = exp.find_groups(mesh)
        for group_par in tmp:
            t = exp.give_group_key(mesh, group_par)
            print "@@@@1:",group_par,t
            if(t.__repr__()!="GROUP_NO"):
               par.append(group_par)
        par=str(par)
        par = par.replace('[','')
        par = par.replace(']','')
        comm.use(WCD.AllMeshGrounp(par))
        ##################
        
        contact=LS.Contact(fric_group+fricless_group)
        
        comm.use(contact)
        #contact.clear_part()
        comm.write(self.comm_path)
        return True
        
    def save_linear(self): #保存到comm文件中
    
        comm = LS.CommWriter()
        comm.use(LS.Modelisation(self.get_sel_itm().get_model_type()))
        
        gm = LS.GrpMaterial()
        gm.clear_part()
        grp_material = comm.use(gm)
        #grp_material.add(LS.Material(self.get_material()))#material
        grp_material.add(LS.Material(self.get_material()))#material
        grp_material.add(LS.GroupMaterial(self.materials_conds))#grouop-material
        

        mech_consts = comm.use(LS.MechConstraints())
        bound_conds = mech_consts.add(LS.BoundConds())
        mesh = self.mesh
        exp = self.exp_store.give_exp("boundaries")
        
        log_gui.debug("save_linear %d", len(self.degrees_conds))
        if len(self.degrees_conds):
            for cond in self.degrees_conds:
                gname = str(cond[0])
                grp_type = exp.give_group_key(mesh, gname)
                bound_conds.add(LS.DplFromName(grp_type, gname, *cond[1:]))
        else:
            mess = "Warning: You have not defined boundary conditions  "
            self.mod.launch(aster_s_gui.INFO, mess)
            
        mech_consts.add(LS.BoundEnd(len("CHARC=AFFE_CHAR_MECA"),");"))
        mech_consts.add(LS.PressStart(0,"CHARF=AFFE_CHAR_MECA(MODELE=MODX,"))

        pressure = mech_consts.add(LS.Pressure() )#here
        forceface = mech_consts.add(LS.ForceFace() )#here 
        forcenodale = mech_consts.add(LS.ForceNodale() )#here
        
        for gname, ptype, v1,v2,v3,v4,v5,v6 in self.pressure_conds:
            if ptype =="Pressure":
                pressure.add(LS.GrpPres(str(gname), v1))
            elif ptype =="Force_Face":
                forceface.add(LS.ForceFaceArgs(str(gname), v1,v2,v3))
            elif ptype =="Force_Node":
                 forcenodale.add(LS.ForceNodaleArgs(str(gname), v1,v2,v3,v4,v5,v6,self.get_dim()))

        comm.write(self.comm_path)
        #self.add_case("linear-static")
        return True
        
class Ast_D_of_F_Model(WC.AstConditionsModel):
    def __init__(self, sel, header_names):
        WC.AstConditionsModel.__init__(self,sel,header_names)
        
    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX)):
            flags = qtc.Qt.ItemIsSelectable
        else:
            flags = self._sel.flaglists[midx.row()][midx.column()]
        #log_gui.debug("flags: %d row: %d column: %d",self._sel.flaglists[midx.row()][midx.column()],midx.row(),midx.column())
        return flags
        
    def setData(self, midx, value,  role = qtc.Qt.EditRole):
        if(role == qtc.Qt.CheckStateRole):
           state,_ = value.toInt()
           self._sel.checklists[midx.row()][midx.column()] = state
           log_gui.debug("setData before: %d row: %d column: %d",self._sel.flaglists[midx.row()][midx.column()],midx.row(),midx.column())
           if (state == qtc.Qt.Unchecked):
              #self._sel.flaglists[midx.row()][midx.column()] &= ~(qtc.Qt.ItemIsEditable) 
              self._sel.flaglists[midx.row()][midx.column()] = self._sel.flaglists[midx.row()][midx.column()] & (~(qtc.Qt.ItemIsEditable) )
              cond = self._sel.give_cond(midx.row())
              cond[midx.column()] = None
           else:
              #self._sel.flaglists[midx.row()][midx.column()] |= qtc.Qt.ItemIsEditable
              self._sel.flaglists[midx.row()][midx.column()] = self._sel.flaglists[midx.row()][midx.column()] | qtc.Qt.ItemIsEditable
              cond = self._sel.give_cond(midx.row())
              cond[midx.column()] = 0
           log_gui.debug("setData: %d row: %d column: %d ",self._sel.flaglists[midx.row()][midx.column()],midx.row(),midx.column())
           
           """row = 0
           for flist in self._sel.flaglists:
               column = 0
               for a in flist:
                   log_gui.debug("row: %d column: %d flag: %d",row,column,self._sel.flaglists[row][column])
                   column += 1
           row += 1"""
           return True
        else:
           log_gui.debug("setData: %d row: %d column: %d",self._sel.flaglists[midx.row()][midx.column()],midx.row(),midx.column())
           return WC.AstConditionsModel.setData(self,value,role)
        
    def data(self, midx, role):
        """Provide data for each cell in the display role"""
        res = QNULL
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX) ):
           if (role  == qtc.Qt.DisplayRole):
              vallist = self._sel.give_default_val()
              res = qtc.QVariant("----")
           elif(role == qtc.Qt.TextAlignmentRole):
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           else:
              res = QNULL
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
           #elif role == qtc.Qt.ForegroundRole:
              #qcolor = qt.QColor(0,240,240)
              #res = qt.QBrush(0,240,240)
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
        else:
           if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
              if (self._sel.checklists[midx.row()][midx.column()] == qtc.Qt.Checked):
                 cond = self._sel.give_cond(midx.row())
                 res = qtc.QVariant(cond[midx.column()])
              else:
                 res = invalided
           elif role == qtc.Qt.TextAlignmentRole:
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           elif role == qtc.Qt.CheckStateRole:
              if (midx.column() == 0):
                 res = QNULL
              else: 
                 res = qtc.QVariant(self._sel.checklists[midx.row()][midx.column()])
           else:
              res = QNULL
        return res
        
class Ast_D_of_F_Selector(WC.AstConditionsSelector):#用于设置degrees of freedom
    """Allow to set Material on proposed groups.
    """
    def __init__(self, data, parent):#condition_type 0 用于degree 2017/2/9
        WC.AstConditionsSelector.__init__(self, data, 0, parent,False)
        self.checklists = data.checklists #二维数组
        self.flaglists = []
        
    def valid_by_group(self):
        if(self._data.grouptypesel != 0):
            self._conds = self._data.degrees_conds
            cexp = self._data.exp_store
            exp = cexp.give_exp("pressure")
            mesh = self._data.mesh
            log_gui.debug("valid_by_group by group type %s, mesh %s", self._data.grouptypesel,mesh)
            grp_names = exp.find_groups(mesh)
            self._grp_names = grp_names
            self._valided = True
            
            dim = self._data.get_dim()
            if (len(grp_names)>0):
               if self._condition_type==0:
                  head_names =[u"Group", u"DX", u"DY"]
                  if (dim == WCD.Dim_Type.Three_Dim):
                     head_names.append(u"DZ")

            checklist = [qtc.Qt.Checked]*len(head_names)
            val_list = [.0]*(len(head_names)-1)
            self._default_cond = [grp_names[0]] + val_list
            self._default_check = checklist
            self._default_flag = [qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled] + [qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled | qtc.Qt.ItemIsUserCheckable] * (len(head_names) - 1) #第一列的数据不需要check 
            #self._default_flag = [qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled] + [qtc.Qt.ItemIsEnabled | qtc.Qt.ItemIsUserCheckable] * (len(head_names) - 1) #第一列的数据不需要check 
            model = Ast_D_of_F_Model(self, head_names)
            self._tab.setModel(model)
            self._tab.setEnabled(True)
            self._tab.horizontalHeader().setClickable(True)
            self._tab.setItemDelegate(WC.ValueDelegate(self))
            self._tab.setItemDelegateForColumn(0,WC.AstGroupDelegate(self,self._grp_names))
            self.add_cond()
            
            icolumn = 0
            for iname in head_names:
                width = 80
                if icolumn > 0:
                   width = 60
                self._tab.setColumnWidth(icolumn,width) 
                icolumn += 1
            self.is_reseted = False
        else:
            self._build()
            
    def add_cond(self):
        """Add a condition to the table"""
        if not self._default_cond:
            return
        model = self._tab.model()
        conds = self._conds
        end_idx = len(conds)
        model.beginInsertRows(qtc.QModelIndex(), end_idx, end_idx)
        conds.append(list(self._default_cond))
        
        log_gui.debug("Ast_D_of_F_Selector conds num:%d",len(conds))
        log_gui.debug("Ast_D_of_F_Selector conds num:%d",len(self._conds))
        log_gui.debug("Ast_D_of_F_Selector conds num:%d",len(self._data.degrees_conds))
        
        self.checklists.append(list(self._default_check))
        self.flaglists.append(list(self._default_flag))
        model.endInsertRows()
        idx = model.index(end_idx, self._groupcolumn, ROOT_MIDX)
        self.notify_wizard()
        
    def remove_cond(self,idx):
        model = self._tab.model()
        model.beginRemoveRows(qtc.QModelIndex(), idx, idx)
        del self._conds[idx]
        del self.flaglists[idx]
        model.endRemoveRows()
        self.emit_datachanged()
        
class Ast_Pressure_Model(WC.AstConditionsModel):
    def __init__(self, sel, header_names):
        WC.AstConditionsModel.__init__(self,sel,header_names)
        
    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX)):
            flags = qtc.Qt.ItemIsSelectable
        else:
            cond = self._sel.give_cond(midx.row())
            if(cond.__contains__(u"Pressure")):
               if(midx.column() > 2):
                 flags = qtc.Qt.ItemIsSelectable
               else:
                 flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
            elif(cond.__contains__(u"Force_Face")):
               if(midx.column() > 4):
                 flags = qtc.Qt.ItemIsSelectable
               else:
                 flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        #log_gui.debug("flags: %d row: %d column: %d",flags,midx.row(),midx.column())
        return flags
        
    def set_pressure_type(self,typename):
        midx = self._sel._tab.currentIndex()
        comb = self.sender()
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX) or midx.column() + 1 == self.columnCount(ROOT_MIDX)):
           return
        else:
           cond = self._sel.give_cond(midx.row())
           cond[midx.column()] = typename
           log_gui.debug("set_pressure_type with want data %s row %s clomn %s cond_data %s", typename, midx.row(), midx.column(), cond)
           self._sel.notify_wizard()
           beg = self.index(midx.row(),midx.column() + 1)
           endind = self.index(midx.row(),midx.column() + 3)
           self.emit(SIG("dataChanged"),beg,endind)
        log_gui.debug("set_pressure_type with want data %s comb %s", typename, comb)
        
    def data(self, midx, role):
        """Provide data for each cell in the display role"""
        res = QNULL
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX) ):
           if (role  == qtc.Qt.DisplayRole):
              vallist = self._sel.give_default_val()
              res = qtc.QVariant("----")
           elif(role == qtc.Qt.TextAlignmentRole):
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           else:
              res = QNULL
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
           #elif role == qtc.Qt.ForegroundRole:
              #qcolor = qt.QColor(0,240,240)
              #res = qt.QBrush(0,240,240)
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
        else:
           cond = self._sel.give_cond(midx.row())
           if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
              if (self.flags(midx) == (qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled)):
                 res = qtc.QVariant(cond[midx.column()])
              else:
                 res = QNULL
           elif role == qtc.Qt.TextAlignmentRole:
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           elif role == qtc.Qt.DecorationRole:
              if (self.flags(midx) == (qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled)):
                 if (cond.__contains__(u"Pressure")):
                    if(midx.column()  == 2):
                       pix = qt.QPixmap(32,32);
                       pix.fill();
                       rct = pix.rect();
                       paint = qt.QPainter(pix);
                       paint.drawText(rct, qtc.Qt.AlignCenter, u"P:");
                       res = pix
                    else: 
                       res = QNULL
                 elif (cond.__contains__(u"Force_Face")):
                    if(midx.column()  > 1):
                       tip = [u"",u"",u"FX:",u"FY:",u"FZ:"]
                       pix = qt.QPixmap(32,32);
                       pix.fill();
                       rct = pix.rect();
                       paint = qt.QPainter(pix);
                       paint.drawText(rct, qtc.Qt.AlignCenter, tip[midx.column()]);
                       res = pix
                    else: 
                       res = QNULL             
                 elif (cond.__contains__(u"Force_Node")):
                    if(midx.column()  > 1):
                       tip = [u"",u"", u"FX:",u"FY:",u"FZ:", u"MX:", u"MY:", u"MZ:"]
                       pix = qt.QPixmap(32,32);
                       pix.fill();
                       rct = pix.rect();
                       paint = qt.QPainter(pix);
                       paint.drawText(rct, qtc.Qt.AlignCenter, tip[midx.column()]);
                       res = pix
                    else: 
                       res = QNULL
              else:
                 res = QNULL
           else:
              res = QNULL
        return res
        
class Ast_Pressure_Selector(WC.AstConditionsSelector):#用于设置degrees of freedom
    """Allow to set Material on proposed groups.
    """
    def __init__(self, data, parent):#condition_type 0 用于degree 2017/2/9
        WC.AstConditionsSelector.__init__(self, data, 0, parent,False)
        self.head_name = [u"Group", u"Pressure\n Type", u"", u"", u"", u"", u""]
        self._pressure_types = [u"Pressure", u"Force_Face", u"Force_Node"]

    def valid_by_group(self):
        self._conds = self._data.pressure_conds
        if(self._data.grouptypesel != 0):
            cexp = self._data.exp_store
            exp = cexp.give_exp("pressure")
            mesh = self._data.mesh
            log_gui.debug("valid_by_group by group type %s, mesh %s", self._data.grouptypesel,mesh)
            grp_names = exp.find_groups(mesh)
            self._grp_names = grp_names
            self._valided = True
            
            dim = self._data.get_dim()
            if (len(grp_names)>0):
               if self._condition_type==0:
                  head_names = copy.copy(self.head_name)
                  if (dim == WCD.Dim_Type.Three_Dim):
                     head_names.append(u"")
            log_gui.debug("valid_by_group head_names: %d",len(head_names))

            val_list = [.0]*(len(head_names) - 2)
            self._default_cond = [grp_names[0], u"Pressure"] + val_list
            model = Ast_Pressure_Model(self, head_names)
            self._tab.setModel(model)
            self._tab.setEnabled(True)
            self._tab.horizontalHeader().setClickable(True)
            self._tab.setItemDelegate(WC.ValueDelegate(self))
            self._tab.setItemDelegateForColumn(0, WC.AstGroupDelegate(self, self._grp_names)) 
            self._tab.setItemDelegateForColumn(1, WC.AstGroupDelegate(self, self._pressure_types)) 
            self.add_cond()
            
            width = [80,90,60,60,60,60,60,60,60,60,60,60]
            icolumn = 0
            for iname in head_names:
                self._tab.setColumnWidth(icolumn,width[icolumn]) 
                icolumn += 1
            self.is_reseted = False
        else:
            self._build()
            
    def add_cond(self):
        """Add a condition to the table"""
        if not self._default_cond:
            return
        model = self._tab.model()
        conds = self._conds
        end_idx = len(conds)
        model.beginInsertRows(qtc.QModelIndex(), end_idx, end_idx)
        conds.append(list(self._default_cond))
        self.notify_wizard()
        model.endInsertRows()
        
    def remove_cond(self,idx):
        model = self._tab.model()
        model.beginRemoveRows(qtc.QModelIndex(), idx, idx)
        del self._conds[idx]
        model.endRemoveRows()
        self.emit_datachanged()
        
class Ast_Pair_Model(Ast_Pressure_Model):

    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX)):
            flags = qtc.Qt.ItemIsSelectable
        else:
            cond = self._sel.give_cond(midx.row())
            if(cond.__contains__(u"bond") or cond.__contains__(u"Frictionless")):
               if(midx.column() > 2):
                 flags = qtc.Qt.ItemIsSelectable
               else:
                 flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        return flags
        
    def data(self, midx, role):
        """Provide data for each cell in the display role"""
        res = QNULL
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX) ):
           if (role  == qtc.Qt.DisplayRole):
              vallist = self._sel.give_default_val()
              res = qtc.QVariant("----")
           elif(role == qtc.Qt.TextAlignmentRole):
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           else:
              res = QNULL
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
           #elif role == qtc.Qt.ForegroundRole:
              #qcolor = qt.QColor(0,240,240)
              #res = qt.QBrush(0,240,240)
              #log_gui.debug("use ForegroundRole role %s and res.type %s", role, type(res))
        else:
           cond = self._sel.give_cond(midx.row())
           if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
              if (self.flags(midx) == (qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled)):
                 res = qtc.QVariant(cond[midx.column()])
              else:
                 res = QNULL
           elif role == qtc.Qt.TextAlignmentRole:
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           else:
              res = QNULL
        return res
        
class Ast_Pair_Selector(WC.AstConditionsSelector):#用于设置pair group definition
    """Allow to set Material on proposed groups.
    """
    def __init__(self, data, parent):
        WC.AstConditionsSelector.__init__(self, data, 0, parent,False)
        
    def valid_by_group(self):
        self._conds = self._data.pair_conds
        if(self._data.grouptypesel != 0):
            cexp = self._data.exp_store
            exp = cexp.give_exp("pressure")
            mesh = self._data.mesh
            log_gui.debug("Ast_Pair_Selector valid_by_group by group type %s, mesh %s", self._data.grouptypesel,mesh)
            grp_names = exp.find_groups(mesh)
            self._grp_names = grp_names
            self._grp_names.reverse()
            self._valided = True
            
            dim = self._data.get_dim()
            head_names =[u"Type", u"Mater group", u"Slave group", u"Friction Coefficient"]
            type_name = [u"bond", u"Frictionless", u"Friction"]

            self._default_cond = [type_name[0]] + [self._grp_names[0]] + [self._grp_names[0]] + [0]
            
            for i in self._default_cond:
              log_gui.debug("Ast_Pair_Selector _default_cond %s", i)

            model = Ast_Pair_Model(self, head_names)
            self._tab.setModel(model)
            self._tab.setEnabled(True)
            self._tab.horizontalHeader().setClickable(True)
            self._tab.setItemDelegate(WC.ValueDelegate(self))
            self._tab.setItemDelegateForColumn(0,WC.AstGroupDelegate(self,type_name))
            self._tab.setItemDelegateForColumn(1,WC.AstGroupDelegate(self,self._grp_names))
            self._tab.setItemDelegateForColumn(2,WC.AstGroupDelegate(self,self._grp_names))
            self.add_cond()
            
            width = [75,80,80,115,60,60,50,50]
            icolumn = 0
            for iname in head_names:
                self._tab.setColumnWidth(icolumn,width[icolumn]) 
                icolumn += 1
            self.is_reseted = False
        else:
            self._build()
        
class Create_Dock(qt.QDockWidget):
    def __init__(self, mod):
        self.use_nolinear = False
        desktop = mod.give_qtwid()
        qt.QDockWidget.__init__(self, desktop)
        self.data = LSData(mod)
        self.data.ini()
        self.exp_store = self.data.exp_store
        
        self.setWindowTitle(u"Linear static")
        centralWidget = qt.QWidget(self)
        vlayout = qt.QVBoxLayout()
        centralWidget.setLayout(vlayout)
        label_for_model = qt.QLabel(u"What kind of model do you want to work on?",self)
        vlayout.addWidget(label_for_model)
        
        model_sel = WC.AstModelSel(self.data,self)
        model_sel.place(vlayout)
        connect(model_sel.combobox,SIG("currentIndexChanged(QString)"),self.model_changed)

        mesh_sel = WC.AstMeshSel(self.data,self)
        mesh_sel.place(vlayout)
        model_sel.add_related_component(mesh_sel)
        
        group_sel = WC.AstGroupTypeSel(self.data,self)
        connect(mesh_sel,SIG("mesh_valided"),group_sel.notify)
        model_sel.add_related_component(group_sel)

        group_sel.place(vlayout)
        
        vspacer = qt.QSpacerItem(20, 10, qt.QSizePolicy.Minimum, qt.QSizePolicy.Expanding)
        vlayout.addItem(vspacer)

        label_for_model = qt.QLabel(u"Adding Material on groups",self)
        vlayout.addWidget(label_for_model)
        #grid = qt.QGridLayout()
        #young = WC.YoungModulus()
        #young.add_to(qt.QWidget(), grid, 0)
        #poisson = WC.PoissonRatio()
        #poisson.add_to(qt.QWidget(), grid, 1)
        #vlayout.addLayout(grid)
        material_sel = WC.AstMaterialSelector(self.data,self)
        material_sel.add_to(vlayout)
        
        vspacer = qt.QSpacerItem(20, 10, qt.QSizePolicy.Minimum, qt.QSizePolicy.Expanding)
        vlayout.addItem(vspacer)

        label_for_model = qt.QLabel(u"Adding imposed degrees of freedom on groups",self)
        vlayout.addWidget(label_for_model)
                
        degrees_sel = Ast_D_of_F_Selector(self.data,self)
        connect(group_sel,SIG("group_valided"),degrees_sel.valid_by_group)
        degrees_sel.add_to(vlayout)
        
        vspacer = qt.QSpacerItem(20, 10, qt.QSizePolicy.Minimum, qt.QSizePolicy.Expanding)
        vlayout.addItem(vspacer)
        
        label_for_model = qt.QLabel(u"Adding pressure or force on meshes groups",self)
        vlayout.addWidget(label_for_model)
        
        pressure_sel = Ast_Pressure_Selector(self.data,self)
        connect(group_sel,SIG("group_valided"),pressure_sel.valid_by_group)
        pressure_sel.add_to(vlayout)
                
        self.check = qt.QCheckBox("Contact pair group definition",self)
        #self.check.hide()
        connect(self.check,SIG("stateChanged(int)"),self.on_use_pair)
        vlayout.addWidget(self.check)

        self.label_for_model = qt.QLabel(u"Adding pair groups",self)
        #self.label_for_model.hide()
        vlayout.addWidget(self.label_for_model)

        self.pair_sel = Ast_Pair_Selector(self.data,self)
        connect(group_sel,SIG("group_valided"),self.pair_sel.valid_by_group)
        #self.pair_sel._tab.hide()
        self.pair_sel.add_to(vlayout)
        
        
        group_sel.add_related_component(degrees_sel)
        group_sel.add_related_component(pressure_sel)
        group_sel.add_related_component(material_sel)
        group_sel.add_related_component(self.pair_sel)
        
        hlayout = qt.QHBoxLayout()
        hspacer = qt.QSpacerItem(27, 20, qt.QSizePolicy.Expanding, qt.QSizePolicy.Minimum)
        hlayout.addItem(hspacer)
        btnok = qt.QPushButton("OK",centralWidget)
        btncancel = qt.QPushButton("Cancel",centralWidget)
        connect(btnok, SIG("clicked()"), self.save_file)
        connect(btncancel, SIG("clicked()"), self.close)
        centralWidget.setMinimumWidth(400);
        hlayout.addWidget(btnok)
        hlayout.addWidget(btncancel)
        vlayout.addLayout(hlayout)
        self.setWidget(centralWidget)
        
        self.model_changed(model_sel.combobox.currentText())

    def on_use_pair(self,state):
        if(state == qtc.Qt.Checked):
          self.use_nolinear = True
          self.pair_sel._tab.show()
        else:
          self.pair_sel._tab.hide()
          self.use_nolinear = False
        log_gui.debug("on_use_pair:%d",state)

    def model_changed(self,model):
        if (self.data.get_dim() == WCD.Dim_Type.Three_Dim):
           self.check.show()
           self.check.setCheckState(qtc.Qt.Checked)
           self.label_for_model.show()
        else:
           self.check.setCheckState(qtc.Qt.Unchecked)
           self.check.hide()
           self.label_for_model.hide()

    def save_file(self):
        self.data.comm_path = WC.load_from_disk(self)
        if(self.use_nolinear):
          self.data.save_nolinear()
        else:
          self.data.save_linear()
        self.close()
        
        
def create_wizard(mod):
    """Create the linear static wizard"""
    wiz = WC.Wizard(u"Isotropic linear elastic study", mod)
    WC.add_model_page(wiz, [
        WC.Mode3D,
        WC.PlaneStress,
        WC.PlaneStrain,
        WC.AxisSymmetric,
        ])

    exp_store = WC.ExpStore()
    exp_store.register(WC.ExpStore.smesh, SMeshExp())
    exp_store.register(WC.ExpStore.geom, GeomExp())
    WC.add_mesh_page(wiz, mod, exp_store)
    title = u"Young's modulus and Poisson ratio definitions"
    WC.add_material_page(wiz, title, [
        WC.YoungModulus(),
        WC.PoissonRatio(),
    ])
    WC.add_boundaries_page(wiz)
    add_pressure_page(wiz)
    WC.add_command_file_page(wiz, FinalPage(mod))
    return wiz


