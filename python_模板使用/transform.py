# -*- coding: utf-8 -*-
import string
import os
import os.path as osp
import sys
from xml.dom.minidom import parse
import xml.dom.minidom

in_file = sys.argv[1]
out_file = sys.argv[2]
bat = sys.argv[3]
resultPath = sys.argv[4]
#resultPath += "\\mom\\results"
#bat = "E:\\program\\RCS\\mom\\run.bat"
#resultPath = "D:\\runrcs\\data\\mom\\results"
'''class Singleton(object):
    """Singleton implementation in python."""
    # add _singleton_id attribute to the class to be independant of import path used
    __inst = {}
    def __new__(cls, *args, **kargs):
        cls_id = getattr(cls, '_singleton_id', cls) 
        if Singleton.__inst.get(cls_id) is None:
            Singleton.__inst[cls_id] = object.__new__(cls)
        return Singleton.__inst[cls_id]'''
        
class PatternLoader():
    """Load command file patterns
    """
    def __init__(self):
        """Initialize the path where patterns are stored"""
        self._path = osp.dirname(__file__)

    def load(self, fname):
        """Load the command file pattern written in the given filename"""
        fid = open(osp.join(self._path, fname))
        pattern = fid.read()
        fid.close()
        return pattern

class Writer(object):
    """The command file writter
    """
    def __init__(self):
        self._data = {}
        self._pattern = None
        
    def load_parttern(self,pattern_file):
        self._pattern = string.Template(PatternLoader().load(pattern_file))
        
    def add_to(self, pattern_key, bloc):
        """Replace the pattern_key by the given bloc"""
        self._data[pattern_key] = bloc

    def write(self, fname):
        """Write the command file in the given filename"""  
        fid = open(fname, "w")
        fid.write(self._pattern.substitute(self._data))
        fid.close()

class Solver_Dom():
    def __init__(self):
      xml_path = in_file
      print in_file
      dom_xml = parse(xml_path)
      self.data_ele = dom_xml.documentElement
      self.writer = Writer()
      self.init()
      
    def init(self):
      Solver_Type = self.data_ele.getElementsByTagName("Solver_Type")
      if(len(Solver_Type)):
        Solver_Type_node = Solver_Type[0]
        sol_type = Solver_Type_node.getAttribute("solver")
      else:
        sol_type = 0

      if(sol_type == "1"): #mom
        self.writer.load_parttern(os.path.join(osp.dirname(__file__),"./simulation_parameters_template.pattern"))
        self._form_mom_args()
      elif(sol_type == "2"): #fdtd
        self.writer.load_parttern(os.path.join(osp.dirname(__file__),"./fdtd_parameters_template.pattern"))
        self._form_fdtd_args()
      elif(sol_type == "3"): #fdtd
        self.writer.load_parttern(os.path.join(osp.dirname(__file__),"./po_parameters_template.pattern"))
        self._form_po_args()
        
    def _form_fdtd_args(self):
      Excitation = self.data_ele.getElementsByTagName("Excitation")
      if(len(Excitation)):
        Excitation_node = Excitation[0]
        fai_range = Excitation_node.getAttribute("fai_range")
        cita_range = Excitation_node.getAttribute("cita_range")
        pol = Excitation_node.getAttribute("polarisation")
      else:
        fai_range = "false"
        cita_range = "false"
        pol = "false"

      #self.writer.add_to("PHI_INC",fai_range)
      #self.writer.add_to("THETA_INC",cita_range)
      #self.writer.add_to("POL",pol)

      Record = self.data_ele.getElementsByTagName("Record")
      if(len(Record)):
        Record_node = Record[0]
        fai_start = Record_node.getAttribute("fai_s_range")      
        fai_end = Record_node.getAttribute("fai_e_range")
        cita_start = Record_node.getAttribute("cita_s_range")      
        cita_end = Record_node.getAttribute("cita_e_range")
      else:
        fai_start = 90
        fai_end = 90
        cita_start = 90
        cita_end = 90

      self.writer.add_to("START_PHI",fai_start)
      self.writer.add_to("STOP_PHI",fai_end)
      self.writer.add_to("START_THETA",cita_start)
      self.writer.add_to("STOP_THETA",cita_end)
      
      Data_dir = self.data_ele.getElementsByTagName("Dir_Info")
      if(len(Data_dir)):
        Data_dir_node = Data_dir[0]
        geom_dir = Data_dir_node.getAttribute("geom_dir")
        geom_name = Data_dir_node.getAttribute("geom_name")
        mesh_name = Data_dir_node.getAttribute("mesh_name")
        mesh_dir = Data_dir_node.getAttribute("mesh_dir")
        stl_dir = mesh_dir + "\\\\" + mesh_name + ".stl" #由于matlab的特殊格式，此处只能用这"\\matlab"这种路径格式
        stl_dir = "\'"+stl_dir+"\'"
      else:
        stl_dir = ''
      #self.writer.add_to("GEOM_DIR",geom_dir)
      #self.writer.add_to("TARGET_NAME",geom_name)
      #self.writer.add_to("MESH_NAME",mesh_name)
      res = "\'"+resultPath+"\'"
      box = sys.argv[5]
      pw_box = float(box) * 1.5 * 1000 #传进来的单位是m，求解器用的是mm
      sim_box = float(box) * 2.0 * 1000
      self.writer.add_to("STL_PATH",stl_dir)
      self.writer.add_to("SIM_PATH",res)
      self.writer.add_to("SIM_BOX",pw_box)
      self.writer.add_to("PW_BOX",sim_box)
      
    def _form_po_args(self):
      Excitation = self.data_ele.getElementsByTagName("Excitation")
      if(len(Excitation)):
        Excitation_node = Excitation[0]
        fai_range = Excitation_node.getAttribute("fai_range")
        cita_range = Excitation_node.getAttribute("cita_range")
        pol = Excitation_node.getAttribute("polarisation")
      else:
        fai_range = "false"
        cita_range = "false"
        pol = "false"

      #self.writer.add_to("PHI_INC",fai_range)
      #self.writer.add_to("THETA_INC",cita_range)
      #self.writer.add_to("POL",pol)

      Record = self.data_ele.getElementsByTagName("Record")
      if(len(Record)):
        Record_node = Record[0]
        fai_start = Record_node.getAttribute("fai_s_range")      
        fai_end = Record_node.getAttribute("fai_e_range")
        cita_start = Record_node.getAttribute("cita_s_range")      
        cita_end = Record_node.getAttribute("cita_e_range")
      else:
        fai_start = 90
        fai_end = 90
        cita_start = 90
        cita_end = 90

      self.writer.add_to("START_PHI",fai_start)
      self.writer.add_to("STOP_PHI",fai_end)
      self.writer.add_to("START_THETA",cita_start)
      self.writer.add_to("STOP_THETA",cita_end)
      
      Data_dir = self.data_ele.getElementsByTagName("Dir_Info")
      if(len(Data_dir)):
        Data_dir_node = Data_dir[0]
        face_name = Data_dir_node.getAttribute("face_name")
        node_name = Data_dir_node.getAttribute("node_name")
        face_name = "\'" + face_name + "\'"
        node_name = "\'" + node_name + "\'"
      else:
        face_name = ''
        node_name = ''
      #self.writer.add_to("GEOM_DIR",geom_dir)
      #self.writer.add_to("TARGET_NAME",geom_name)
      #self.writer.add_to("MESH_NAME",mesh_name)
      res = "\'"+resultPath+"\'"
      self.writer.add_to("GEOM_FACE_PATH",face_name)
      self.writer.add_to("GEOM_NODE_PATH",node_name)
      self.writer.add_to("RES_Path",res)
      
    def _form_mom_args(self):
      self.writer.add_to("MONOSTATIC_RCS",1)	    
      self.writer.add_to("MONOSTATIC_SAR",0)
      
      wizard = self.data_ele.getElementsByTagName("Wizard")
      if(len(wizard)):
        wizard_node = wizard[0]
        module_type = wizard_node.getAttribute("module_type")
        page = wizard_node.getElementsByTagName(module_type)
      else:
        module_type = "false"
        page = "false"
      '''if(len(wizard)):
        page_node = page[0]
        e_field = page_node.getAttribute("e_field")
        h_field = page_node.getAttribute("h_field")
      else:
        e_field = "false"
        h_field = "false"

      HH = 0
      VV = 0
      HV = 0
      VH = 0
      if(e_field == "2" and h_field == "2" ):
          HH = 1
          VV = 1
          HV = 0
          VH = 0
      elif(e_field != "2" and h_field == "2" ):
          HH = 0
          VV = 0
          HV = 1
          VH = 0
      elif(e_field == "2" and h_field != "2" ):
          HH = 0
          VV = 0
          HV = 0
          VH = 1      
      elif(e_field != "2" and h_field != "2" ):
          HH = 0
          VV = 0
          HV = 0
          VH = 0
      elif(e_field != "2" and h_field != "2" ):
          HH = 0
          VV = 0
          HV = 0
          VH = 0'''
      
      self.writer.add_to("HH",1)
      self.writer.add_to("VV",1)
      self.writer.add_to("VH",0)
      self.writer.add_to("HV",0)
      Excitation = self.data_ele.getElementsByTagName("Excitation")
      if(len(Excitation)):
        Excitation_node = Excitation[0]
        fai_range = Excitation_node.getAttribute("fai_range")
        cita_range = Excitation_node.getAttribute("cita_range")
        pol = Excitation_node.getAttribute("polarisation")
      else:
        fai_range = "false"
        cita_range = "false"
        pol = "false"

      self.writer.add_to("PHI_INC",fai_range)
      self.writer.add_to("THETA_INC",cita_range)
      self.writer.add_to("POL",pol)

      Record = self.data_ele.getElementsByTagName("Record")
      if(len(Record)):
        Record_node = Record[0]
        fai_start = Record_node.getAttribute("fai_s_range")      
        fai_end = Record_node.getAttribute("fai_e_range")
        cita_start = Record_node.getAttribute("cita_s_range")      
        cita_end = Record_node.getAttribute("cita_e_range")
      else:
        fai_start = 0
        fai_end = 0
        cita_start = 0
        cita_end = 0

      self.writer.add_to("START_PHI",fai_start)
      self.writer.add_to("STOP_PHI",fai_end)
      self.writer.add_to("START_THETA",cita_start)
      self.writer.add_to("STOP_THETA",cita_end)
      
      Data_dir = self.data_ele.getElementsByTagName("Dir_Info")
      if(len(Data_dir)):
        Data_dir_node = Data_dir[0]
        geom_dir = Data_dir_node.getAttribute("geom_dir")
        geom_name = Data_dir_node.getAttribute("geom_name")
        mesh_name = Data_dir_node.getAttribute("mesh_name")
        mesh_dir = Data_dir_node.getAttribute("mesh_dir")
        mesh_dir = "\""+mesh_dir+"\""
        geom_dir = "\""+geom_dir+"\""
        geom_name = "\""+geom_name+"\""
        mesh_name = "\""+mesh_name+"\""
      else:
        mesh_dir = 0
        geom_dir = 0
        geom_name = 0
        mesh_name = 0
      self.writer.add_to("GEOM_DIR",geom_dir)
      self.writer.add_to("TARGET_NAME",geom_name)
      self.writer.add_to("MESH_NAME",mesh_name)
      self.writer.add_to("MESH_DIR",mesh_dir)
      
    def save(self, f_name):
      self.writer.write(f_name)
      
dom = Solver_Dom()
dom.save(out_file)
print out_file
print in_file
print bat
print resultPath

import subprocess
subprocess.call([bat, resultPath])
##isar pictures generation after all compution complete




