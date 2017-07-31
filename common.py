# encoding: utf-8

"""Common objects for creating graphical wizards
"""
import os
import os.path as osp

from PyQt4 import QtCore as qtc
from PyQt4 import QtGui as qt
connect = qtc.QObject.connect
SIG = qtc.SIGNAL
Policy = qt.QSizePolicy
AbsTableModel = qtc.QAbstractTableModel
QNULL = qtc.QVariant()
ROOT_MIDX = qtc.QModelIndex()
import aster_s.wizards.common as WCD

import salome
import aster_s
import aster_s.salome_tree as ST
import aster_s.wizards.common as WC
from aster_s.utils import log_gui

import aster_s_gui
import aster_s_gui.common as GC


class WizardField(object):
    """Store a Python object for a field in the Qt wizard mechanism.
    If the field is mandatory, it will be considered complete when
    its value is different from the initial one.
    """
    _sig = SIG("clicked()")

    def __init__(self, wizard_page, name, initial_value):
        # A PushButton is used just for its signal without argument 'clicked()'
        # because custom signals can not be used:
        #
        # from PyQt4 import QtCore as qtc
        # from PyQt4 import QtGui as qt
        # app = qt.QApplication([])
        # wid = qt.QWidget()
        # page = qt.QWizardPage()
        # page.registerField("field*", wid, "prp", qtc.SIGNAL("updateField()"))
        # wiz = qt.QWizard()
        # wiz.addPage(page)
        # -> Object::connect: No such signal QWidget::updateField()
        #
        wid = qt.QPushButton()
        wid.setProperty("python-object", initial_value)
        # The Qt mechanism will check the property value when this signal
        # is emitted.
        wizard_page.registerField(name, wid, "python-object", self._sig)
        self._store_widget = wid
        self._name = name
        log_gui.debug("registerField (%s) with initial value : %s", repr(name), initial_value)

    def update(self, value):
        """Update the field value and run the qt Wizard checking mechanism
        """
        log_gui.debug("update value of field %s with : %s", repr(self._name), value)
        wid = self._store_widget
        wid.setProperty("python-object", value)
        wid.emit(self._sig)

    def give_value(self):
        """Return the value stored by the field"""
        return self._store_widget.property("python-object")


class WizardPage(qt.QWizardPage):
    """Custom wizard page returning Python object and having reference
    on its parent page
    """

    def __init__(self):
        qt.QWizardPage.__init__(self)
        self.page = None

    def get_int(self, name):
        """Return Qt field as integer"""
        return self.field(name).toInt()[0]

    def get_float(self, name):
        """Return Qt field as float"""
        return self.field(name).toDouble()[0]

    def get_str(self, name):
        """Return Qt field as str"""
        return str(self.field(name).toString())

    def give_field(self, name):
        """Return the python object for the field name"""
        return self.field(name).toPyObject()

    def cleanup(self):
        """Clean page in case of user navigation"""
        for child in self.children():
            child.deleteLater()

    def initialize(self):
        """Initialize page when page is asked to be shown"""
        assert not self.children()


class Page(object):
    """A wizard page
    """

    def __init__(self, wizard, title, subtitle, wiz_page=None):
        wid = wiz_page or WizardPage()
        wid.page = self
        wid.setTitle(title)
        wid.setSubTitle(subtitle)
        wizard.addPage(wid)
        self._wid = wid
        self._fields = {}

    def register(self, name, initial_value):
        """Register a field in Qt wizard mechanism with its initial value"""
        field = WizardField(self._wid, name, initial_value)
        self._fields[name] = field
        return field

    def give(self, name):
        """Give the registered wizard field from its name"""
        return self._fields[name]

    def register_qt_field(self, name, widget):
        """Register a field with its Qt widget in the Qt wizard mechanism.
        The widget is supposed to be correctly handled by the QtWizardPage,
        else it should be easier to use the register method."""
        self._wid.registerField(name, widget)

    def use(self, layout):
        """Use the given layout"""
        self._wid.setLayout(layout)
        return layout


class Wizard(qt.QWizard):
    """Linear static wizard
    """

    def __init__(self, title, mod):
        qt.QWizard.__init__(self, mod.give_qtwid())
        self._title = title
        self._mod = mod
        self._pages = []

    def add_page(self, title, wiz_page=None):
        """Add a wizard page"""
        page = Page(self, self._title, title, wiz_page)
        self._pages.append(page)
        return page

    def run(self):
        """Run the wizard"""
        if not self._mod.testing:
            self.show()


class Validator(object):
    """Build a Qt validator from specifications
    """

    def build(self, parent):
        """Build the qt validator"""
        raise NotImplementedError


class Int(Validator):
    """Int validator
    """

    def __init__(self, minv, maxv):
        self._minv = minv
        self._maxv = maxv

    def build(self, parent):
        """Build a QIntValidator"""
        return qt.QIntValidator(self._minv, self._maxv, parent)


class Double(Validator):
    """Double validator
    """

    def __init__(self, minv, maxv, decs_nb=8):
        self._minv = minv
        self._maxv = maxv
        self._dnb = decs_nb

    def build(self, parent):
        """Build a QDoubleValidator"""
        return qt.QDoubleValidator(self._minv, self._maxv, self._dnb, parent)


class ValidationEntry(qt.QLineEdit):
    """Change the color background if the value is valid
    """
    valid_color = "white"
    invalid_color = "red"

    def __init__(self, parent=None):
        qt.QLineEdit.__init__(self, parent)
        connect(self, SIG("textChanged(const QString&)"), self._update_color)
        self._update_color(self.text())

    def _update_color(self, txt):
        """Update the color according to input"""
        color = self.valid_color
        if not self.hasAcceptableInput():
            color = self.invalid_color
        self.setStyleSheet("background-color: %s" % color)


def create_entry(validator):
    """Create a QLineEdit with a validator using given specifications"""
    entry = ValidationEntry()
    entry.setValidator(validator.build(entry))
    return entry


class ConditionsModel(AbsTableModel):
    """Qt table model storing conditions
    """

    def __init__(self, sel, header_names):
        AbsTableModel.__init__(self)
        self._sel = sel
        self._headers = [qtc.QVariant(name) for name in header_names]
        self._cols_nb = len(self._headers)
        
    def rowCount(self, parent_midx):
        """Return the number of rows to Qt mechanism when the table
        changes"""
        return self._sel.give_conds_nb()

    def columnCount(self, parent_midx):
        """Return the number of columns to Qt mechanism when the table
        changes"""
        return self._cols_nb

    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        return qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled

    def data(self, midx, role):
        """Provide data for each cell in the display role"""
        res = QNULL
        if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
            cond = self._sel.give_cond(midx.row())
            res = qtc.QVariant(cond[midx.column()])
        return res

    def headerData(self, sidx, orientation, role):
        """Tell to Qt mechanism the header data for building table view"""
        res = None
        if role == qtc.Qt.DisplayRole:
            res = QNULL
            if orientation == qtc.Qt.Horizontal:
                res = self._headers[sidx]
        elif role == qtc.Qt.DecorationRole and orientation == qtc.Qt.Vertical:
            res = qtc.QVariant(GC.load_icon("wizards/remove.png"))
        if res is None:
            res = AbsTableModel.headerData(self, sidx, orientation, role)
        return res


class   GroupDelegate(qt.QStyledItemDelegate):
    """Allow to select a group in a table cell by using a QComboBox
    """

    def __init__(self, sel, grp_names):
        qt.QStyledItemDelegate.__init__(self, sel)
        self._sel = sel
        self._grp_names = grp_names
        
    def createEditor(self, parent, options, midx):
        """Create a QComboBox"""
        return qt.QComboBox(parent)
        
    def setEditorData(self, combo, midx):
        """Set the group names and select the current one"""
        grps = self._grp_names
        combo.addItems(grps)
        cond = self._sel.give_cond(midx.row())
        combo.setCurrentIndex(grps.index(cond[midx.column()]))
        
    def setModelData(self, combo, model, midx):
        """Set the selected group on selector and update wizard"""
        sel = self._sel
        cond = sel.give_cond(midx.row())
        cond[midx.column()] = self._grp_names[combo.currentIndex()]
        sel.notify_wizard()
        
class   AstGroupDelegate(qt.QStyledItemDelegate): #added by zxq 实现选择一个group后将当前单元格向后移动一个
    """Allow to select a group in a table cell by using a QComboBox
    """

    def __init__(self, sel, grp_names):
        qt.QStyledItemDelegate.__init__(self, sel)
        self._sel = sel
        self._grp_names = grp_names
        
    def createEditor(self, parent, options, midx):
        """Create a QComboBox"""
        return qt.QComboBox(parent)
        
    def setEditorData(self, combo, midx):
        """Set the group names and select the current one"""
        grps = self._grp_names
        combo.addItems(grps)
        cond = self._sel.give_cond(midx.row())
        combo.setCurrentIndex(grps.index(cond[midx.column()]))
        combo.setProperty("row",midx.row())
        combo.setProperty("column",midx.column())
        connect(combo,SIG("currentIndexChanged(int)"),self.setSelected)
    
    def setSelected(self,indx):
        combo = self.sender()
        row,_ = combo.property("row").toInt()
        column,_ = combo.property("column").toInt()
        model = self._sel._tab.model()
        if(column + 1 == model.columnCount(ROOT_MIDX) and row + 2 == model.rowCount(ROOT_MIDX)): #若当前单元格为右下角最后一个，则选定右上角第一个
          indx = model.index(0,0)
        elif(column + 1 < model.columnCount(ROOT_MIDX)): #否则选择同行的下一个
          indx = model.index(row,column+1)
        else: #当前列若为最后一列时则选择下一行的首列
          indx = model.index(row + 1, 0)
        self._sel._tab.setCurrentIndex(indx)

    def setModelData(self, combo, model, midx):
        """Set the selected group on selector and update wizard"""
        sel = self._sel
        cond = sel.give_cond(midx.row())
        cond[midx.column()] = self._grp_names[combo.currentIndex()]
        sel.notify_wizard()


class ValueValidator(qt.QDoubleValidator):
    """Allow to enter an empty string
    """

    def validate(self, txt, pos):
        """Validate the edited text"""
        state, rpos = qt.QDoubleValidator.validate(self, txt, pos)
        if txt.length() == 0:
            state = qt.QValidator.Acceptable
        return state, rpos

class TextDelegate(qt.QStyledItemDelegate):
    """Allow to select a value in a table cell : size of value is 8
    """

    def __init__(self, sel):
        qt.QStyledItemDelegate.__init__(self, sel)
        self._sel = sel

    def createEditor(self, parent, options, midx):
        """Create QLineEdit with validator"""
        ledit = qt.QLineEdit(parent)
        rx = qtc.QRegExp()
        rx.setPattern("\\S{0,8}");
        validator = qt.QRegExpValidator(rx, ledit)
        ledit.setValidator(validator)
        return ledit

    def setEditorData(self, ledit, midx):
        """Set the value from the model"""
        cond = self._sel.give_cond(midx.row())
        val = cond[midx.column()]
        txt = ""
        if val is not None:
            txt = str(val)
        ledit.setText(txt)
        
    def setModelData(self, ledit, model, midx):
        """Store the value on the model"""
        sel = self._sel
        val = None
        txt = str(ledit.text())
        if txt :
            val=txt
        cond = sel.give_cond(midx.row())
        cond[midx.column()] = val
        sel.notify_wizard()
        

class ValueDelegate(qt.QStyledItemDelegate):
    """Allow to select a value in a table cell or None
    """

    def __init__(self, sel, decimals_nb=6, vrange=(-1e9, 1e9)):
        qt.QStyledItemDelegate.__init__(self, sel)
        self._sel = sel
        self._decimals_nb = decimals_nb
        self._vrange = vrange

    def createEditor(self, parent, options, midx):
        """Create QLineEdit with validator"""
        ledit = qt.QLineEdit(parent)
        vmin, vmax = self._vrange
        dnb = self._decimals_nb
        ledit.setValidator(ValueValidator(vmin, vmax, dnb, ledit))
        return ledit

    def setEditorData(self, ledit, midx):
        """Set the value from the model"""
        cond = self._sel.give_cond(midx.row())
        val = cond[midx.column()]
        txt = ""
        if val is not None:
            txt = str(val)
        ledit.setText(txt)

    def setModelData(self, ledit, model, midx):
        """Store the value on the model"""
        sel = self._sel
        val = None
        txt = str(ledit.text())
        if txt:
            val = float(txt)
        cond = sel.give_cond(midx.row())
        cond[midx.column()] = val
        sel.notify_wizard()


class ConditionsSelector(qtc.QObject):
    """Allow to set conditions on proposed groups.
    The conditions are built in a list of lists given to the WizardField.
    """

    def __init__(self, title, wfield, grp_names, dims,  new_grp="Group", grp_colm="None"):#argument "wfield" does not need to be assigned  zxq 2017/2/9
        qtc.QObject.__init__(self)
        self._title = title
        self._wfield = wfield
        self._grp_colm=grp_colm
        self._cont = qt.QWidget()
        self._tab = qt.QTableView()
        self._grp_names =grp_names
        if self._grp_colm=="New_coln":
            self._grp_names0 =  self._grp_names[0]
            self._grp_names1 =  self._grp_names[1]
            names =["Crack Name", "Crack geometry"]
            self._model = ConditionsModel(self, names)
            dcond = None
            if grp_names:
                dcond =["Fiss_01", self._grp_names1[0]]
        else :
            self._grp_names = grp_names
            names = [new_grp] + [dim[0] for dim in dims]
            self._model = ConditionsModel(self, names)
            dcond = None
            if grp_names:
                dcond = [grp_names[0]] + [dim[1] for dim in dims] 
        self._default_cond = dcond
        self._conds = []
        self._build()

    def _build(self):
        """Build the condition selector"""
        tab = self._tab
        tab.setModel(self._model)
        tab.horizontalHeader().setClickable(False)
        sig = SIG("sectionClicked(int)")
        connect(tab.verticalHeader(), sig, self.remove_cond)
        if self._grp_colm=="New_coln":
            tab.setItemDelegate(TextDelegate(self))
            tab.setItemDelegateForColumn(1, AstGroupDelegate(self,self._grp_names1))

        else :
            tab.setItemDelegate(ValueDelegate(self))
            tab.setItemDelegateForColumn(0, AstGroupDelegate(self, self._grp_names)) 
        
    def give_conds_nb(self):
        """Return the number of conditions"""
        return len(self._conds)

    def give_cond(self, idx):
        """Return the condition list from its index"""
        return self._conds[idx]

    def notify_wizard(self):
        """Notify the wizard mechanism that a change occured"""
        if (self._wfield != None):
           self._wfield.update(self._conds or None)

    def add_cond(self):
        """Add a condition to the table"""
        if not self._default_cond:
            return
        model = self._model
        conds = self._conds
        end_idx = len(conds)
        model.beginInsertRows(ROOT_MIDX, end_idx, end_idx)
        conds.append(list(self._default_cond))
        model.endInsertRows()
        self.notify_wizard()

    def remove_cond(self, idx):
        """Remove a condition from its index"""
        model = self._model
        model.beginRemoveRows(ROOT_MIDX, idx, idx)
        del self._conds[idx]
        model.endRemoveRows()
        self.notify_wizard()

    def add_to(self, main_lay):
        """Add the table to the layout and keep the selector
        alive by using the layout parent widget
        """
        cont = self._cont
        lay = qt.QHBoxLayout(cont)
        lay.addWidget(GC.create_icon_button("wizards/add.png", self.add_cond))
        lab = qt.QLabel(self._title)
        lab.setSizePolicy(Policy.Expanding, Policy.Preferred)
        lay.addWidget(lab)
        if not self._default_cond:
            cont.setEnabled(False)

        main_lay.addWidget(cont)
        main_lay.addWidget(self._tab)
        self.setParent(main_lay.parentWidget())

    def deleteLater(self):
        """Delete selector has well as its own widgets"""
        self._cont.deleteLater()
        self._tab.deleteLater()
        qtc.QObject.deleteLater(self)


def add_condition_selector(wiz_page, grp_names, dims, field_name, title, new_grp="Group", grp_colm="None"):
    """Add condition selector on a wizard page for the given
    data groups. The wizard page must have a layout and a mesh"""
    wfield = wiz_page.page.give(field_name)
    sel = ConditionsSelector(title, wfield, grp_names,dims, new_grp, grp_colm)

    # Add a least a condition if the field is mandatory
    if field_name.endswith("*"):
        sel.add_cond()
    sel.add_to(wiz_page.layout())

Mode3DV = (WC.Mode3DV, "Volumetric elements")
Mode3D = (WC.Mode3D, "3D")
PlaneStress = (WC.PlaneStress, "Plane stress")
PlaneStrain = (WC.PlaneStrain, "Plane strain")
AxisSymmetric = (WC.AxisSymmetric, "Axis symmetric")
Plane = (WC.Plane, "Plane")
Ele_filaire = (WC.Ele_filaire, "Wire elements")
Ele_surface = (WC.Ele_surface, "Plate and shell elements") 
POU_D_E = (WC.POU_D_E, "Euler beam")
POU_D_T = (WC.POU_D_T, "Timoshenko beam")
DKT = (WC.DKT, "DKT (Discrete Kirchhoff Theory)")
DST = (WC.DST, "DST (Discrete Shear Theory)")
COQUE_3D = (WC.COQUE_3D, "Volumetric shell")

class ModelSelection(qt.QComboBox):
    """Allow to select an Aster model
    """

    def __init__(self, wfield, choices):
        qt.QComboBox.__init__(self)
        self._wfield = wfield
        self._choices = choices
        self.addItems([choice[1] for choice in choices])
        connect(self, SIG("currentIndexChanged(int)"), self._update_model)

    def _update_model(self, idx):
        """Update the Aster model type from user choice"""
        self._wfield.update(self._choices[idx][0])


def add_model_page(wiz, choices):
    """Add the model selection page"""
    page = wiz.add_page(u"Model definition")
    lay = page.use(qt.QVBoxLayout())
    lay.addWidget(qt.QLabel(u"What kind of model do you want to work on?"))
    wfield = page.register("model", choices[0][0])
    lay.addWidget(ModelSelection(wfield, choices))
   

def add_model_case(wiz, choices):
    """Add the model selection page"""
    page = wiz.add_page(u"Model definition")
    lay = page.use(qt.QVBoxLayout())
    lay.addWidget(qt.QLabel(u"What kind of model do you want to work on?"))
    wfield = page.register("model", choices[0][0])
    lay.addWidget(ModelSelection(wfield, choices))


class MeshSelection(qt.QWidget):
    """Mesh selection
    """
    no_dim_mess = "The selected mesh has no dimension"
    dim_mess = "The mesh dimension of %i does not match the model one of %i"

    def __init__(self, mod, model, wfield, exp_sel):
        qt.QWidget.__init__(self)
        self._mod = mod
        self._model = model
        self._wfield = wfield
        self._exp_sel = exp_sel
        icon_name = "load-browser-entry.png"
        self._but = GC.create_icon_button(icon_name, self._load_msh)
        self._lab = qt.QLabel()
        self._build_gui()

    def _build_gui(self):
        """Build gui with button and label"""
        box = qt.QHBoxLayout(self)
        box.addWidget(self._but)

        lab = self._lab
        Pol = qt.QSizePolicy
        lab.setSizePolicy(Pol.Expanding, Pol.Preferred)
        lab.setFrameStyle(qt.QLabel.Panel)
        box.addWidget(lab)

    def _load_msh(self):
        """Load mesh from Aster module"""
        log_gui.debug("_load_msh begin")
        mod = self._mod
        mesh = mod.load_mesh_from_selection()
        if not mesh:
            mod.launch(GC.ERROR, "The selected entry is not a mesh "
                "or the SMESH component must be activated")
            return
        mdim = mesh.give_dim()
        if (mdim != self._model.dim):
            mess = self.no_dim_mess
            if type(mdim) is int:
                mess = self.dim_mess % (mdim, self._model.dim)
            mod.launch(GC.ERROR, mess)
            return
        self._lab.setText(mesh.read_name())
        self._exp_sel.notify(mesh)
        log_gui.debug("_load_msh update field with mesh = %s", mesh)
        self._wfield.update(mesh)
        log_gui.debug("_load_msh end")

class MeshSelectionWdg(qt.QWidget):#added by zxq use to replace MeshSelection
    """Mesh selection
    """
    def __init__(self, mod, parent = 0):
        qt.QWidget.__init__(self,parent)
        self._mod = mod
        icon_name = "load-browser-entry.png"
        self._but = GC.create_icon_button(icon_name, self._load_msh)
        self._lab = qt.QLabel()
        self._build_gui()

    def _build_gui(self):
        """Build gui with button and label"""
        box = qt.QHBoxLayout(self)
        box.addWidget(self._but)

        lab = self._lab
        Pol = qt.QSizePolicy
        lab.setSizePolicy(Pol.Expanding, Pol.Preferred)
        lab.setFrameStyle(qt.QLabel.Panel)
        box.addWidget(lab)

    def _load_msh(self):
        """Load mesh from Aster module"""
        log_gui.debug("_load_msh begin")
        mod = self._mod
        mesh = mod.load_mesh_from_selection()
        if not mesh:
            mod.launch(GC.ERROR, "The selected entry is not a mesh "
                "or the SMESH component must be activated")
            return 
        self._lab.setText(mesh.read_name())
        self._exp_sel.notify(mesh)
        log_gui.debug("_load_msh end")

class CompoExp(object):
    """Wizard contract for a component explorator
    """

    def __init__(self):
        self._exps = {}

    def register(self, physical_key, exp):
        """Register an explorator for a physcial key"""
        self._exps[physical_key] = exp

    def give(self, physical_key):
        """Give the explorator for the physical key"""
        return self._exps[physical_key]

    def validate(self, mesh, mod):
        """Validate the mesh before continuing the wizard"""
        return True


class ExpStore(object):
    """Store component explorators
    """
    smesh, geom = [object() for oidx in range(2)]

    def __init__(self):
        self.compo_type = self.smesh
        self._exps = {}

    def use(self, compo_type):
        """The given component type"""
        self.compo_type = compo_type

    def register(self, compo_type, compo_exp):
        """Register a component explorator for the given component type"""
        self._exps[compo_type] = compo_exp

    def give_compo_exp(self):
        """Return the explorator of the component type"""
        return self._exps[self.compo_type]

    def give_exp(self, physical_key):
        """Give the explorator from the given physical key"""
        return self.give_compo_exp().give(physical_key)


class ExploratorSelector(qt.QWidget):
    """Allow to select the mesh explorator for the SMESH or GEOM
    component
    """
    desc = u"Use %s groups"

    def __init__(self, mod, exp_store):
        qt.QWidget.__init__(self)
        self._mod = mod
        self._exp_store = exp_store
        self._use_mesh = qt.QRadioButton(self.desc % u"mesh")
        self._use_geom = qt.QRadioButton(self.desc % u"geometrical")
        self._build()

    def _build(self):
        """Build its GUI"""
        grp = qt.QButtonGroup(self)
        lay = qt.QVBoxLayout(self)
        for but in (self._use_mesh, self._use_geom):
            but.setEnabled(False)
            grp.addButton(but)
            lay.addWidget(but)
        self._use_mesh.setChecked(True)

    def notify(self, mesh):
        """Allow to use geometrical groups on selected mesh if possible"""
        umesh, ugeom = self._use_mesh, self._use_geom
        umesh.setEnabled(True)
        umesh.setChecked(True)

        ugeom.setEnabled(False)
        if mesh.has_geom():
            ugeom.setEnabled(True)
            ugeom.setChecked(True)

    def accept(self, mesh):
        """Accept the mesh is valid"""
        ugeom = self._use_geom
        ctype = ExpStore.smesh
        if ugeom.isEnabled() and ugeom.isChecked():
            ctype = ExpStore.geom
        self._exp_store.use(ctype)

        exp = self._exp_store.give_compo_exp()
        is_valid = False
        if exp.validate(mesh, self._mod):
            is_valid = True
        log_gui.debug("ExploratorSelector.accept returns %s", is_valid)
        return is_valid

class ExploratorSelectorWdg(qt.QWidget):#added by zxq use to replace ExploratorSelector
    """Allow to select the mesh explorator for the SMESH or GEOM
    component
    """
    desc = u"Use %s groups"

    def __init__(self, mod,parent = 0):
        qt.QWidget.__init__(self,parent)
        self._mod = mod
        #self._exp_store = exp_store
        self._use_mesh = qt.QRadioButton(self.desc % u"mesh")
        self._use_geom = qt.QRadioButton(self.desc % u"geometrical")
        self._build()

    def _build(self):
        """Build its GUI"""
        grp = qt.QButtonGroup(self)
        lay = qt.QVBoxLayout(self)
        for but in (self._use_mesh, self._use_geom):
            grp.addButton(but)
            lay.addWidget(but)
        self._use_mesh.setChecked(True)

mesh_field = "mesh"

class MeshPage(WizardPage):
    """Page for selecting a mesh
    """

    def __init__(self, mod):
        WizardPage.__init__(self)
        self._mod = mod
        self._esel = None

    def cleanupPage(self):
        """Clean page in case of user navigation"""
        WizardPage.cleanup(self)
        self.page.give(mesh_field).update(None)

    def initializePage(self):
        """Check that the mesh and model matches and check
        for mesh groups"""
        WizardPage.initialize(self)
        page = self.page
        lay = page.use(qt.QVBoxLayout())

        title = u"Select a mesh from the Salomé object browser"
        lay.addWidget(qt.QLabel(title))
        model = self.give_field("model")
        wfield = page.give(mesh_field)
        exp_store = self.give_field("exp-store")
        esel = ExploratorSelector(self._mod, exp_store)
        msel = MeshSelection(self._mod, model, wfield, esel)
        lay.addWidget(msel)
        lay.addWidget(esel)
        self._esel = esel

    def validatePage(self):
        """Tell the mesh to use meshes or geometrical groups"""
        log_gui.debug("validateMeshPage called")
        mesh = self.give_field("mesh")
        log_gui.debug("mesh : %s", mesh)
        return self._esel.accept(mesh)

class MeshWdg(qt.QWidget):#added by zxq use to replace class MeshPage
    """for selecting a mesh
    """
    def __init__(self, mod,parent = 0):
        qt.QWidget.__init__(self,parent)
        self._mod = mod
        """Check that the mesh and model matches and check
        for mesh groups"""
        lay = qt.QVBoxLayout()
        self.setLayout(lay)
        lay.addWidget(qt.QLabel(u"Select a mesh from the Salome object browser"))
        esel = ExploratorSelectorWdg(self._mod,self)
        msel = MeshSelectionWdg(self._mod,self)
        lay.addWidget(msel)
        lay.addWidget(esel)
        self._esel = esel
        
class Material_Wdg(qt.QWidget):#added by zxq use to replace add_material_page()
    def __init__(self, parent = 0):
         qt.QWidget.__init__(self,parent)
         lay = qt.QVBoxLayout()
         self.setLayout(lay)
         lay.addWidget(qt.QLabel(u"Young's modulus and Poisson ratio definitions"))
         wdg = qt.QWidget()
         lay.addWidget(wdg)
         grid = qt.QGridLayout(wdg)
         young = YoungModulus()
         young.add_to(wdg, grid, 0)
         poisson = PoissonRatio()
         poisson.add_to(wdg, grid, 1)

class Bounaries_Conditions_Wdg(qt.QWidget):#added by zxq use to replace BoundariesPage
    def __init__(self, mod, exp, typename, parent = 0):
         qt.QWidget.__init__(self,parent)
         lay = qt.QVBoxLayout()
         self.setLayout(lay)
         if typename == "presure":
           dims = [("DX", 0.), ("DY", 0.), ("DZ", 0.)]
           tit = u"Adding imposed degrees of freedom on groups"
         else:
           dims = [(u"Pressure", 1.)]
           tit = u"Adding pressure on meshes groups"
         grp_names = exp.find_groups(mod.load_mesh_from_selection())
         sel = ConditionsSelector(tit, 0, grp_names,dims, "Group", "None")
         sel.add_cond()
         sel.add_to(lay)


def at_least_a_group(exp, mesh, mod):
    """A valid mesh needs to have at least a node or mesh groups"""
    is_valid = True
    if not exp.find_groups(mesh):
        mess = "At least a group needs to be defined on the selected object"
        mod.launch(GC.ERROR, mess)
        is_valid = False
    return is_valid

def add_mesh_page(wiz, mod, exp_store):
    """Add the mesh selection page"""
    page = wiz.add_page(u"Mesh selection", MeshPage(mod))
    page.register(mesh_field, None)
    page.register("exp-store", exp_store)


class Param(object):
    """Parameter for building page in a grid layout
    """

    def add_to(self, page, grid, ridx):
        """Add to the page and grid layout at the row index"""
        raise NotImplementedError


class YoungModulus(Param):
    """Young's modulus parameter
    """

    def __init__(self, dft="2.1e11", rangev=(0., 10e100)):
        self._dft = dft
        self._range = rangev
        self.title = u"Young's modulus (E)"
        self.suffix = u"(E >= 0)"

    def add_to(self, page, grid, ridx):
        """Add young modulus"""
        grid.addWidget(qt.QLabel(self.title), ridx, 0)
        minv, maxv = self._range
        entry = create_entry(Double(minv, maxv))
        if (page != None):
            page.register_qt_field("young-modulus*", entry)
        entry.setText(self._dft)
        grid.addWidget(entry, ridx, 1)
        grid.addWidget(qt.QLabel(self.suffix), ridx, 2)

class ShearModulus(Param):
    """Young's modulus parameter
    """

    def __init__(self, dft="2.1e5", rangev=(0., 10e100), title = u"Sheer modulus"):
        self._dft = dft
        self._range = rangev
        self.title = title
        self.suffix = u"(G >= 0)"

    def add_to(self, page, grid, ridx):
        """Add young modulus"""
        grid.addWidget(qt.QLabel(self.title), ridx, 0)
        minv, maxv = self._range
        entry = create_entry(Double(minv, maxv))
        page.register_qt_field("young-modulus*", entry)
        entry.setText(self._dft)
        grid.addWidget(entry, ridx, 1)
        grid.addWidget(qt.QLabel(self.suffix), ridx, 2)


class PoissonRatio(Param):
    """Poisson's ratio parameter
    """

    def __init__(self, dft="0.3"):
        self._dft = dft
        self.rangev = (-1., 0.5)
        self.decs_nb = 10
        nu_symb =  u"\N{GREEK SMALL LETTER NU}"
        self.title = u"Poisson's ratio (%s)" % nu_symb
        self.suffix = u"(-1 <= %s <= 0.5)" % nu_symb

    def add_to(self, page, grid, ridx):
        """Add Poisson's ratio"""
        grid.addWidget(qt.QLabel(self.title), ridx, 0)
        minv, maxv = self.rangev
        entry = create_entry(Double(minv, maxv, self.decs_nb))
        if (page != None):
            page.register_qt_field("poisson-ratio*", entry)
        entry.setText(self._dft)
        grid.addWidget(entry, ridx, 1)
        grid.addWidget(qt.QLabel(self.suffix), ridx, 2)


class Density(Param):
    """Material density parameter
    """

    def __init__(self, dft="7.8e3"):
        self._dft = dft
        self._rangev = (1e-10, 1e100)
        self.decs_nb = 10
        rho_symb = u"\N{GREEK SMALL LETTER RHO}"
        self.title = u"Density (%s)" % rho_symb
        self.suffix = u"(%s > 0)" % rho_symb

    def add_to(self, page, grid, ridx):
        """Add density"""
        grid.addWidget(qt.QLabel(self.title), ridx, 0)
        minv, maxv = self._rangev
        entry = create_entry(Double(minv, maxv, self.decs_nb))
        if (page != None):
            page.register_qt_field("density*", entry)
        entry.setText(self._dft)
        grid.addWidget(entry, ridx, 1)
        grid.addWidget(qt.QLabel(self.suffix), ridx, 2)


def add_grid_page(wiz, title, stitle, params):
    """Add a page with a grid layout"""
    page = wiz.add_page(title)
    lay = page.use(qt.QVBoxLayout())
    lay.addWidget(qt.QLabel(stitle))
    cont = qt.QWidget()
    lay.addWidget(cont)
    grid = qt.QGridLayout(cont)
    for ridx, param in enumerate(params):
        param.add_to(page, grid, ridx)

def add_material_page(wiz, title, params):
    """Add the page defining material properties"""
    add_grid_page(wiz, u"Material properties", title, params)


class BoundariesPage(WizardPage):
    """Wizard page on boundaries condition
    """

    def cleanupPage(self):
        """Clean page in case user navigates"""
        WizardPage.cleanup(self)

    def initializePage(self):
        """Query the model and mesh for nodes and element groups"""
        WizardPage.initialize(self)
        self.page.use(qt.QVBoxLayout())

        dims = [("DX", 0.), ("DY", 0.)]
        if self.give_field("model").dim == 3:
            dims.append(("DZ", 0.))
        exp = self.give_field("exp-store").give_exp("boundaries")
        lst = exp.find_groups(self.give_field("mesh"))
        tit = u"Adding imposed degrees of freedom on groups"
        add_condition_selector(self, lst, dims, "group-boundaries*", tit)


def add_boundaries_page(wiz):
    """Add page on boundaries conditions"""
    page = wiz.add_page(u"Boundaries conditions", BoundariesPage())
    page.register("group-boundaries*", None)


class FinalPage(WizardPage):
    """Final page writting command file
    """

    def __init__(self, mod):
        WizardPage.__init__(self)
        self._mod = mod

    def find_new_name(self, std, name):
        """Find a new name for the wizard"""
        all_names = [case.read_name() for case in std.get_all(aster_s.Case)]
        new_name = name
        for idx in xrange(100):
            if new_name not in all_names:
                return new_name
            new_name = name + str(idx)
        else:
            mod.launch(ERROR, "Too many wizards '%s' in use" % name)

    def add_case(self, name):
        """Add the Aster study case with the given name. 'command-file'
        and 'mesh' fields must be registered"""
        mod = self._mod
        std = mod.give_aster_study()
        prefs = aster_s_gui.AsterPreferences()
        
        case = std.add_case(self.find_new_name(std, name))
        case.use(aster_s.CommFile(self.get_str("command-file")))
        case.use(aster_s.SMeshEntry(self.give_field("mesh").node.entry))
        if prefs.get(aster_s_gui.InteractiveFollowUp):
            case.use(aster_s.InteractivFollowUp())
        if prefs.get(aster_s_gui.SaveBaseResult):
            case.use(aster_s.HasBaseResult())
        mod.update()
        #salome.sg.updateObjBrowser(0)

class CommandFileEntry(qt.QLineEdit):
    """Display command file entry
    """
    title = u"Command file selection"
    ext = "comm"
    filter = u"Command file (*.%s)" % ext

    def __init__(self):
        qt.QLineEdit.__init__(self)
        self.setReadOnly(True)

    def load_from_disk(self):
        """Load command file from disk"""
        dia = qt.QFileDialog(self, self.title)
        dia.setAcceptMode(qt.QFileDialog.AcceptSave)
        dia.setDefaultSuffix(self.ext)
        dia.setFilter(self.filter)
        if dia.exec_():
            fname = dia.selectedFiles()[0]
            fname = fname.replace(qtc.QString("/"), qtc.QString("\\"))
            self.setText(fname)
        dia.deleteLater()


def add_command_file_page(wiz, final_page):
    """Add the page for choosing the filename for writting the
    command file"""
    page = wiz.add_page(u"Filename for writting command file", final_page)
    lay = page.use(qt.QHBoxLayout())
    lay.addWidget(qt.QLabel(u"Aster command file"))
    entry = CommandFileEntry()
    lay.addWidget(entry)
    page.register_qt_field("command-file*", entry)
    but = GC.create_icon_button("load-file.png", entry.load_from_disk)
    lay.addWidget(but)

"""added by zxq 2017.1.13"""
class AstComboBox(qt.QComboBox):
    """Combobox for selecting model """
    def __init__(self,parent):
        qt.QComboBox.__init__(self,parent)

        self.setView(qt.QTreeView())
        self.view().setHeaderHidden(True)
        self.view().setItemsExpandable(False)
        self.view().setRootIsDecorated(False)

    def setmodel(self,itemlist):
        print itemlist
        pmodel = qt.QStandardItemModel()
        self.setModel(pmodel)
        for item in itemlist:
          piterm =  qt.QStandardItem(item.name)
          piterm.setSelectable(item.selectable)
          for child in item.childen:
               pchilditem = qt.QStandardItem(child.name)
               pchilditem.setSelectable(child.selectable)
               piterm.appendRow(pchilditem)
          pmodel.appendRow(piterm)
          
    def showPopup(self):
        self.setRootModelIndex(qtc.QModelIndex())
        self.view().expandAll()
        qt.QComboBox.showPopup(self)
        
class AstModelSel(WC.AstObject):
    def __init__(self, data, parent):
        WC.AstObject.__init__(self,parent)
        self.data = data
        self.itemlist = data.modellist.itemlist
        self.data.itemindex = u"defult"
        self.combobox = AstComboBox(parent)
        self.combobox.setmodel(self.itemlist)
        self.combobox.setCurrentIndex( 0)
        connect(self.combobox, SIG("currentIndexChanged(QString)"), self.setdata)
        
    def reset(self):
        self.setdata(u"defult")
        pass
        
    def place(self,layout):
        layout.addWidget(self.combobox)
        
    def setdata(self,dataindex):
        self.data.itemindex = dataindex
        self.emit_datachanged()
        print("select model begin xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx %s", dataindex)

class AstMeshSel(WC.AstObject):
    no_dim_mess = "The selected mesh has no dimension"
    dim_mess = "The mesh dimension of %i does not match the model one of %i"

    def __init__(self, data, parent):
        WC.AstObject.__init__(self, parent)
        self._data = data
        self._mod = data.mod
 
        icon_name = "load-browser-entry.png"
        self._but = GC.create_icon_button(icon_name, self._load_msh)
        self._lab = qt.QLabel()
        self._layout = self._build_gui()
        
    def _build_gui(self):
        """Build gui with button and label"""
        vlayout = qt.QVBoxLayout()
        
        box = qt.QHBoxLayout()
        box.addWidget(self._but)

        lab = self._lab
        Pol = qt.QSizePolicy
        lab.setSizePolicy(Pol.Expanding, Pol.Preferred)
        lab.setFrameStyle(qt.QLabel.Panel)
        box.addWidget(lab)
        title = u"Select a mesh from the Salomé object browser"
        vlayout.addWidget(qt.QLabel(title))
        vlayout.addLayout(box)
        return vlayout

    def _load_msh(self):
        """Load mesh from Aster module"""
        log_gui.debug("_load_msh begin")
        mod = self._mod
        mesh = mod.load_mesh_from_selection()
        if not mesh:
            mod.launch(GC.ERROR, "The selected entry is not a mesh "
                "or the SMESH component must be activated")
            return 
        mdim = mesh.give_dim()
        if (mdim != self._data.get_dim()):
            mess = self.no_dim_mess
            if type(mdim) is int:
                mess = self.dim_mess % (mdim, self._data.get_dim())
            mod.launch(GC.ERROR, mess)
            return
            
        self.setdata(mesh)
        log_gui.debug("_load_msh end")

    def reset(self):
        self.setdata(None)

    def place(self,layout):
        layout.addLayout(self._layout)
        
    def setdata(self,mesh):
        self._data.mesh = mesh
        if (mesh):
          self._lab.setText(mesh.read_name())
          self.emit(SIG("mesh_valided"))
        else:
          self._lab.setText("")
        self.emit_datachanged()

class AstData():
    def __init__(self,mod):
        self.modellist = WCD.Model_Type_List(WCD.Analysis_Type.Linear_Elatic)
        self.itemindex = "defult"
        
        self.mesh = None
        self.mod = mod
        exp_store = ExpStore()
        #exp_store.register(ExpStore.smesh, SMeshExp())
        #exp_store.register(ExpStore.geom, GeomExp())
        self.exp_store = exp_store
        self.grouptypesel = 0 #0表示不可用，1使用几何，2使用网格
        self.material_names = ["defult1","defult1"]
        self.checklists = [] #作为二维数组使用 每行的第一个数据仅站位使用，无实际意义 任何时候都不应该使用self.checklists[i][0]的数据，目的是为了与conds数据对齐，使用方便 
        self.materials_conds = []
        self.degrees_conds = []
        self.pressure_conds = []
        self.pair_conds = []
        self.comm_path = ""
        
    def get_dim(self):
        return self.get_sel_itm().dim
        
    def get_material(self): #材料名称，杨氏系数和泊松比
        demo1 = ["defult1", 200000.0,0.3]
        demo2 = ["defult2", 200000.0,0.3]
        material = []
        material.append(demo1)
        material.append(demo2)
        return material
        
    def get_sel_itm(self):
        return (self.modellist.get_item(self.itemindex))

class AstGroupTypeSel(WC.AstObject):
    """Allow to select the mesh explorator for the SMESH or GEOM
    component
    """
    desc = u"Use %s groups"
    def __init__(self, data, parent):
        WC.AstObject.__init__(self, parent)
        self._data = data
        self._exp_store = data.exp_store
        self._use_mesh = qt.QRadioButton(self.desc % u"mesh")
        self._use_geom = qt.QRadioButton(self.desc % u"geometrical")
        self._use_invalided = qt.QRadioButton(self.desc % u"invalided")
        
        connect(self._use_mesh, SIG("toggled(bool)"), self.meshaccept)
        connect(self._use_geom, SIG("toggled(bool)"), self.geomaccept)
        self._layout = self._build()
        
    def place(self,layout):
        layout.addLayout(self._layout)

    def _build(self):
        """Build its GUI"""
        grp = qt.QButtonGroup(self)
        lay = qt.QHBoxLayout()
        for but in (self._use_mesh, self._use_geom):
            but.setEnabled(False)
            grp.addButton(but)
            lay.addWidget(but)
        grp.addButton(self._use_invalided)
        self._use_invalided.hide()
        return lay

    def notify(self):
        """Allow to use geometrical groups on selected mesh if possible"""
        _mesh = self._data.mesh
        log_gui.debug("notify mesh is valided %s", _mesh)
        if(_mesh != None):
          umesh, ugeom = self._use_mesh, self._use_geom
          umesh.setEnabled(True)
          #umesh.setChecked(True)
          ugeom.setEnabled(False)
          if _mesh.has_geom():
            ugeom.setEnabled(True)
            #ugeom.setChecked(True)
        else:
          self.reset()

    def meshaccept(self, checked):
        """Accept the mesh is valid"""
        if checked:
           if (self._data.mesh):
              ctype = ExpStore.smesh
              self._exp_store.use(ctype)
              exp = self._exp_store.give_compo_exp()
              is_valid =  exp.validate(self._data.mesh, self._data.mod)
              if (is_valid):
                 self.setdata(1)
              else:
                 self._use_invalided.setChecked(True)
                 self.setdata(0)
                 #self.reset()
           log_gui.debug("ExploratorSelector.accept returns %s and use mesh group", is_valid)
        
    def geomaccept(self, checked):
        """Accept the mesh is valid"""
        if checked:
           if (self._data.mesh != None):
              ctype = ExpStore.geom
              self._exp_store.use(ctype)
              exp = self._exp_store.give_compo_exp()
              is_valid =  exp.validate(self._data.mesh, self._data.mod)
              if (is_valid):
                 self.setdata(2)
              else:
                 self._use_invalided.setChecked(True)
                 self.setdata(0)
                 #self.reset()
           log_gui.debug("ExploratorSelector.accept returns %s and use geom group", is_valid)
        
    def reset(self):
        self.setdata(0)
        self._use_invalided.setChecked(True)
        self._use_mesh.setEnabled(False)
        self._use_geom.setEnabled(False)
        
    def setdata(self, grouptype):
        self._data.grouptypesel = grouptype #0表示不可用，1表示使用mesh_groupﺿ表示geom_group
        if(bool(grouptype)):
          self.emit(SIG("group_valided"))
        self.emit_datachanged()

class AstConditionsModel(AbsTableModel):
    """Qt table model storing conditions
    """

    def __init__(self, sel, header_names):
        AbsTableModel.__init__(self)
        self._sel = sel
        self._headers = [qtc.QVariant(name) for name in header_names]
        self._cols_nb = len(self._headers)

    def rowCount(self, parent_midx):
        """Return the number of rows to Qt mechanism when the table
        changes"""
        return self._sel.give_conds_nb() + 1

    def columnCount(self, parent_midx):
        """Return the number of columns to Qt mechanism when the table
        changes"""
        return self._cols_nb

    def flags(self, midx):
        """Tell to Qt mechanism that each cell is enabled and editable"""
        flags = qtc.Qt.ItemIsEditable | qtc.Qt.ItemIsEnabled
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX)):
            flags = qtc.Qt.ItemIsSelectable
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
           if role in (qtc.Qt.DisplayRole, qtc.Qt.EditRole):
              cond = self._sel.give_cond(midx.row())
              res = qtc.QVariant(cond[midx.column()])
           elif role == qtc.Qt.TextAlignmentRole:
              res = qtc.QVariant(qtc.Qt.AlignHCenter | qtc.Qt.AlignVCenter)
           else:
              res = QNULL
        return res

    def headerData(self, sidx, orientation, role):
        """Tell to Qt mechanism the header data for building table view"""
        res = None
        if orientation == qtc.Qt.Vertical and role == qtc.Qt.DecorationRole and sidx == self._sel.give_conds_nb():
            res = qtc.QVariant(GC.load_icon("wizards/add.png"))
        elif role == qtc.Qt.DisplayRole:
            res = QNULL
            if orientation == qtc.Qt.Horizontal:
                res = self._headers[sidx]
        elif role == qtc.Qt.DecorationRole and orientation == qtc.Qt.Vertical:
            res = qtc.QVariant(GC.load_icon("wizards/remove.png"))
        if res is None:
            res = AbsTableModel.headerData(self, sidx, orientation, role)
        return res
        
class AstMaterialModel(AstConditionsModel):
    def __init__(self, sel, header_names):
        AstConditionsModel.__init__(self,sel, header_names)
        
    def setmaterial(self,text):
        midx = self._sel.get_view().currentIndex()
        comb = self.sender()
        if (midx.row() + 1 == self.rowCount(ROOT_MIDX) or midx.column() + 1 == self.columnCount(ROOT_MIDX)):
           return
        else:
           cond = self._sel.give_cond(midx.row())
           cond[midx.column()] = text
           log_gui.debug("setgroup with want data %s cond_data %s", text, cond)
           self._sel.notify_wizard()
        log_gui.debug("setmaterial with want data %s comb %s", text, comb)

class AstConditionsSelector(WC.AstObject):
    """Allow to set conditions on proposed groups.
    The conditions are built in a list of lists given to the WizardField.
    """
    def __init__(self, data, condition_type, parent, use_btn = False):#condition_type 0 表示用于自由度， 1 表示用于压力zxq 2017/2/9
        WC.AstObject.__init__(self, parent)
        self._data = data
        self.is_reseted = False
        self._condition_type = condition_type
        self._tab = qt.QTableView(parent)
        self._grp_names =[]
        self._connect = False
        self._conds = []
        self._default_model = AstConditionsModel(self, ["----","----"])
        self._build()
        self._default_cond = ["----", "----"]
        if (use_btn):
          self._btns = []
        else:
          self._btns = None

        self._groupcolumn = 0
        if condition_type > 1:
           self._groupcolumn = 1
    def _build(self):
        """Build the condition selector"""
        tab = self._tab
        tab.setModel(self._default_model)
        tab.setEnabled(False)
        tab.horizontalHeader().setClickable(False)
        if not self._connect:
          sig = SIG("sectionClicked(int)")
          self.connect(tab.verticalHeader(), sig, self.setdata)
          self._connect = True
    def give_conds_nb(self):
        """Return the number of conditions"""
        return len(self._conds)
        
    def give_cond(self, idx):
        """Return the condition list from its index"""
        return self._conds[idx]

    def add_cond(self):
        """Add a condition to the table"""
        if not self._default_cond:
            return
        model = self._tab.model()
        conds = self._conds
        end_idx = len(conds)
        model.beginInsertRows(qtc.QModelIndex(), end_idx, end_idx)
        conds.append(list(self._default_cond))
        btn = None
        if (self._btns != None):
            btn = qt.QPushButton("Preview",self._tab)
            self._btns.append(btn)
            connect(btn, SIG("clicked()"),self.preview)
        model.endInsertRows()
        
        if (btn != None):
          idx = model.index(end_idx, len(conds)+1, ROOT_MIDX)
          self._tab.setIndexWidget(idx, btn)
        self.notify_wizard()

    def preview(self):
        pass
        
    def setdata(self, idx):
        if(idx == self.give_conds_nb()):
           log_gui.debug("add a row by index = %s total = %s", idx, self.give_conds_nb())
           self.add_cond()
        else:
           """Remove a condition from its index"""
           log_gui.debug("remove a row by index = %s total = %s", idx, self.give_conds_nb())
           self.remove_cond(idx)
           
    def get_view(self):
        return self._tab
    def give_default_val(self):
           return self._default_cond
           
    def remove_cond(self,idx):
        model = self._tab.model()
        model.beginRemoveRows(qtc.QModelIndex(), idx, idx)
        del self._conds[idx]
        if (self._btns != None):
            del self._btns[idx]
        model.endRemoveRows()
        self.emit_datachanged()
           
    def add_to(self, main_lay):
        """Add the table to the layout and keep the selector
        alive by using the layout parent widget
        """
        main_lay.addWidget(self._tab)
        self.setParent(main_lay.parentWidget())
                
    def valid_by_group(self):
        if(self._data.grouptypesel != 0):
            cexp = self._data.exp_store
            exp = cexp.give_exp("pressure")
            mesh = self._data.mesh
            log_gui.debug("valid_by_group by group type %s, mesh %s", self._data.grouptypesel,mesh)
            grp_names = exp.find_groups(mesh)
            dim = self._data.get_dim()
            if (len(grp_names)>0):
               if self._condition_type==0:
                  head_names =[u"Group", u"DX", u"DY"]
                  self._default_cond = [grp_names[0]] + [0.0, 0.0]
                  if (dim == WC.Dim_Type.Three_Dim):
                     head_names.append(u"DZ")
                     self._default_cond.append(0.0)
               else:
                     head_names = [u"Group", u"Presure"]
                     self._default_cond = [grp_names[0]] + [1]
               self._grp_names = grp_names
               model = AstConditionsModel(self, head_names)
               self._tab.setModel(model)
               self._tab.setEnabled(True)
               self._tab.horizontalHeader().setClickable(True)
               self._tab.setItemDelegate(ValueDelegate(self))
               self.add_cond()
               self._tab.setItemDelegateForColumn(0, AstGroupDelegate(self, self._grp_names))
               icolumn = 0
               for iname in head_names:
                   width = 80
                   if icolumn > 0:
                      width = 60
                   self._tab.setColumnWidth(icolumn,width) 
                   icolumn += 1
               self.is_reseted = False
            else:
               return
        else:
            self._build()
            
    def notify_wizard(self):
        """Notify  that a change occured"""
        self.emit_datachanged()
        #self.emit(SIG("condition_update"), self._conds or None)

    def reset(self):#所有数据置0
        if not self.is_reseted:
           model = self._tab.model()
           model.beginRemoveRows(qtc.QModelIndex(), 0, self.give_conds_nb())
           del self._conds[:]
           model.endRemoveRows()
        
           self._conds = []
           self._default_model = AstConditionsModel(self, ["----","----"])
           self._default_cond = ["----", "----"]
           self._grp_names =[]
           self._dim = self._data.get_dim()
           self.emit_datachanged()
           self.is_reseted = True
        self.valid_by_group()
        

def load_from_disk(parent):
    title = u"Command file selection"
    ext = "comm"
    filter = u"Command file (*.%s)" % ext
    """Load command file from disk"""
    dia = qt.QFileDialog(parent,title)
    dia.setAcceptMode(qt.QFileDialog.AcceptSave)
    dia.setDefaultSuffix(ext)
    dia.setFilter(filter)
    fname = ""
    if dia.exec_():
        fname = dia.selectedFiles()[0]
        fname = fname.replace(qtc.QString("/"), qtc.QString("\\"))
    dia.deleteLater()
    return fname
            
class AstMaterialSelector(AstConditionsSelector):#用于设置group 的材料（每个材料的名字是唯一的）
    """Allow to set Material on proposed groups.
    """
    def __init__(self, data, parent):#condition_type 0  2017/2/9
        AstConditionsSelector.__init__(self, data, 1, parent, False)
        self._material_names = data.material_names
    def valid_by_group(self):
        self._conds = self._data.materials_conds
        if(self._data.grouptypesel != 0):
            cexp = self._data.exp_store
            exp = cexp.give_exp("pressure")
            mesh = self._data.mesh
            log_gui.debug("valid_by_group by group type %s, mesh %s", self._data.grouptypesel,mesh)
            grp_names = exp.find_groups(mesh)
            dim = self._data.get_dim()
            if (len(grp_names)>0):
               if self._condition_type==0:
                  head_names =[u"Group", u"DX", u"DY"]
                  self._default_cond = [grp_names[0]] + [0.0, 0.0]
                  if (dim == WC.Dim_Type.Three_Dim):
                     head_names.append(u"DZ")
                     self._default_cond.append(0.0)
               else:
                     head_names = [u"Group", u"Material"]
                     self._default_cond = [grp_names[0]] + [self._material_names[0]]
               self._grp_names = grp_names
               model = AstMaterialModel(self, head_names)
               self._tab.setModel(model)
               self._tab.setEnabled(True)
               self._tab.horizontalHeader().setClickable(True)
               self.add_cond()
               self._tab.setItemDelegateForColumn(0, AstGroupDelegate(self, self._grp_names))
               self._tab.setItemDelegateForColumn(1, AstGroupDelegate(self, self._material_names))
               icolumn = 0
               for iname in head_names:
                   width = 80
                   if icolumn > 0:
                      width = 60
                   self._tab.setColumnWidth(icolumn,width) 
                   icolumn += 1
               self.is_reseted = False
            else:
               return
        else:
            self._build()
            
    def remove_cond(self,idx):
        model = self._tab.model()
        model.beginRemoveRows(qtc.QModelIndex(), idx, idx)
        del self._conds[idx]
        model.endRemoveRows()
        self.emit_datachanged()
        
    def add_cond(self):
        """Add a condition to the table"""
        if not self._default_cond:
            return
        model = self._tab.model()
        conds = self._conds
        end_idx = len(conds)
        model.beginInsertRows(qtc.QModelIndex(), end_idx, end_idx)
        conds.append(list(self._default_cond))
        model.endInsertRows()
        self.notify_wizard()