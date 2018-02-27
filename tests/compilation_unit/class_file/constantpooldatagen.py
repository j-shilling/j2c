#!/usr/bin/python3

import sys
import os
import re
import subprocess
import shutil
import xml.etree.ElementTree as ET

class ConstantPoolItem:

    def __init__ (self, text):
        args = text.split()

        self.index = int (args[0][1:])
        self.type = args[2]

        if self.type == "Class":
            self.name_index = int (args[3][1:])

        elif self.type == "Fieldref" or self.type == "Methodref" or self.type == "InterfaceMethodref":
            vals = args[3].split('.');

            self.class_index = vals[0][1:]
            self.name_and_type_index = vals[1][1:]

        elif self.type == "String":
            self.string_index = args[3][1:]

        elif self.type == "Integer" or self.type == "Long":
            self.value = int (args[3].replace("l",""))

        elif self.type == "Float" or self.type == "Double":
            self.value = float (args[3].replace("d","").replace("f",""))

        elif self.type == "NameAndType":
            vals = args[3].split(':')

            self.name_index = int (vals[0][1:])
            self.descriptor_index = int (vals[1][1:])

        elif self.type == "Utf8":
            if (len (args) > 3):
                self.value = args[3]
            else:
                self.value = ""

        elif self.type == "MethodHandle":
            vals = args[3].split (':')

            self.reference_kind = vals[0][1:]
            self.reference_index = vals[1][1:]

        elif self.type == "MethodType":
            self.descriptor_index = args[3][1:]

        elif self.type == "InvokeDynamic":
            vals = args[3].split (':')

            self.bootstrap_method_attr_index = vals[0][1:]
            self.name_and_type_index = vals[1][1:]

        else:
            raise ValueError ("Unrecognized type \"" + self.type + "\"")
            
    def __dir__ (self):
        if self.type == "Class":
            return ['index', 'type', 'name_index']
        elif self.type == "Fieldref" or self.type == "Methodref" or self.type == "InterfaceMethodref":
            return ['index', 'type', 'class_index', 'name_and_type_index']
        elif self.type == "String":
            return ['index', 'type', 'string_index']
        elif self.type == "Integer" or self.type == "Long" or self.type == "Float" or self.type == "Double" or self.type == "Utf8":
            return ['index', 'type', 'value']
        elif self.type == "NameAndType":
            return ['index', 'type', 'name_index', 'descriptor_index']
        elif self.type == "MethodHandle":
            return ['index', 'type', 'reference_kind', 'reference_index']
        elif self.type == "MethodType":
            return ['index', 'type', 'descriptor_index']
        elif self.type == "InvokeDynamic":
            return ['index', 'type', 'bootstrap_method_attr_index', 'name_and_type_index']
        else:
            raise ValueError ("Unrecognized type \"" + self.type + "\"")

    def __str__ (self):
        attrs = dir (self)
        ret = ""
        for attr in attrs:
            ret = ret + " " + attr + " = \"" + str (getattr (self, attr)) + "\""

        return ret

    def attach (self, parent):
        elmt = ET.SubElement (parent, str (self.index), { 'type':self.type })
        attrs = dir (self)
        attrs.remove ('index')
        attrs.remove ('type')

        for attr in attrs:
            node = ET.SubElement (elmt, attr)
            node.text = str (getattr (self, attr))

        return elmt

jre = os.environ['JAVA_HOME']
if os.path.isdir (os.path.join (jre, "jre")):
    jre = os.path.join (jre, "jre")

jt = os.path.join (jre, "lib", "rt.jar")
if not os.path.isfile (jt):
    raise RuntimeError ("Cannot find rt.jar")

cwd = os.path.abspath (os.getcwd())

rtdir = os.path.join (cwd, "rt.jar")
if os.path.isdir (rtdir):
    print ("Removing old rt.jar files")
    shutil.rmtree (rtdir)

outdir = os.path.join (cwd, "constant_pool_data")
if os.path.isdir (outdir):
    print ("Removing old result files")
    shutil.rmtree (outdir)

os.makedirs (rtdir)
print ("Extracting rt.jar")
subprocess.run (["jar", "xvf", jt], cwd=rtdir, stdout = subprocess.DEVNULL)

os.chdir (cwd)

for root, dirs, files in os.walk (rtdir, followlinks = True):
    for name in files:
        if not name.endswith (".class") or name == "package-info.class":
            continue

        print ("Parsing ", name)

        path = os.path.join (root, name)
        out = os.path.join (outdir, name.replace(".class", ".constant_pool.xml"))

        result = subprocess.run (
            ["javap", "-verbose", path],
            stdin = subprocess.DEVNULL, stdout = subprocess.PIPE, stderr = subprocess.DEVNULL).stdout.decode ('utf-8')

        cp = ET.Element ('constant_pool', { 'file': os.path.abspath (path) } )
        tree = ET.ElementTree(cp)

        inConstantPool = False
        constantPool = [];
        for line in result.splitlines():
            if (inConstantPool):
                if (line[0] == '{'):
                    inConstantPool = False
                else:
                    constantPool.append(line)
            else:
                inConstantPool = line.startswith("Constant pool")

        for line in constantPool:
            try:
                item = ConstantPoolItem (line)
                elmnt = item.attach (cp)

            except ValueError as e:
                print ("[WARNING] in ", path, ": ", e)

        os.makedirs (os.path.dirname (out), exist_ok=True)
        tree.write (out)
