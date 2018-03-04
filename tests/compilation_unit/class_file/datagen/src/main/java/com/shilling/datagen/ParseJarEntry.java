/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.shilling.datagen;

import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.apache.bcel.classfile.ClassParser;
import org.apache.bcel.classfile.Constant;
import org.apache.bcel.classfile.ConstantCP;
import org.apache.bcel.classfile.ConstantClass;
import org.apache.bcel.classfile.ConstantDouble;
import org.apache.bcel.classfile.ConstantFloat;
import org.apache.bcel.classfile.ConstantInteger;
import org.apache.bcel.classfile.ConstantInvokeDynamic;
import org.apache.bcel.classfile.ConstantLong;
import org.apache.bcel.classfile.ConstantMethodHandle;
import org.apache.bcel.classfile.ConstantMethodType;
import org.apache.bcel.classfile.ConstantNameAndType;
import org.apache.bcel.classfile.ConstantPool;
import org.apache.bcel.classfile.ConstantString;
import org.apache.bcel.classfile.ConstantUtf8;
import org.apache.bcel.classfile.JavaClass;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 *
 * @author Jake Shilling <shilling.jake@gmail.com>
 */
public class ParseJarEntry implements Runnable {
    
    private final Logger logger = LogManager.getLogger(ParseJarEntry.class);
    private final File dest_dir;
    private final File class_file;
    
    public ParseJarEntry (File dest_dir, File class_file) {
        this.class_file = class_file;
        this.dest_dir = dest_dir;
        
        this.dest_dir.mkdirs();
    }

    @Override
    public void run() {
        this.logger.trace ("Parsing " + this.class_file.getName());
        try {
            JavaClass clazz = 
                    new ClassParser (this.class_file.getAbsolutePath())
                            .parse();
            Document doc = DocumentBuilderFactory
                    .newInstance()
                    .newDocumentBuilder()
                    .newDocument();
            Element rootElement = doc.createElement("class");
            doc.appendChild(rootElement);
            rootElement.setAttribute("path", this.class_file.getAbsolutePath());
            
            Element minorElement = doc.createElement("minor_version");
            minorElement.setTextContent(Integer.toString(clazz.getMinor()));
            rootElement.appendChild(minorElement);
            
            Element majorElement = doc.createElement("major_version");
            majorElement.setTextContent(Integer.toString(clazz.getMajor()));
            rootElement.appendChild(majorElement);
            
            Element constantPoolElement = doc.createElement("constant_pool");
            rootElement.appendChild(constantPoolElement);
            
            ConstantPool constantPool = clazz.getConstantPool();
            constantPoolElement.setAttribute("constant_pool_count", Integer.toString(constantPool.getLength()));
            
            for (int i = 0; i < constantPool.getLength(); i++) {
                Constant constant = constantPool.getConstant(i);
                if (constant == null)
                    continue;
                
                Element element = doc.createElement("constant");
                constantPoolElement.appendChild(element);
                element.setAttribute("index", Integer.toString(i));
                element.setAttribute("tag", Byte.toString(constant.getTag()));
                
                switch (constant.getTag()) {
                    case 7: /* Class */
                        Element name = doc.createElement ("name_index");
                        element.appendChild(name);
                        name.setTextContent( Integer.toString(((ConstantClass) constant).getNameIndex()) );
                        break;
                        
                    case 9: /* Fieldref */
                    case 10: /* Methodref */
                    case 11: /* InterfaceMethodref */
                        Element classIndex = doc.createElement ("class_index");
                        element.appendChild (classIndex);
                        classIndex.setTextContent( Integer.toString(((ConstantCP)constant).getClassIndex()) );
                        
                        Element nameAndType = doc.createElement ("name_and_type_index");
                        element.appendChild(nameAndType);
                        nameAndType.setTextContent( Integer.toString(((ConstantCP)constant).getNameAndTypeIndex()) );
                        break;
                        
                    case 8: /* String */
                        Element index = doc.createElement ("string_index");
                        element.appendChild(index);
                        index.setTextContent( Integer.toString(((ConstantString)constant).getStringIndex()) );
                        break;
                        
                    case 3: /* Integer */
                        element.setTextContent( Integer.toString(((ConstantInteger)constant).getBytes()) );
                        break;
                        
                    case 4: /* Float */
                        element.setTextContent( Float.toString(((ConstantFloat)constant).getBytes()) );
                        break;
                        
                    case 5: /* Long */
                        element.setTextContent( Long.toString(((ConstantLong)constant).getBytes()) );
                        break;
                        
                    case 6: /* Double */
                        element.setTextContent( Double.toString(((ConstantDouble)constant).getBytes()) );
                        break;
                        
                    case 12: /* NameAndType */
                        Element nameIndex = doc.createElement("name_index");
                        element.appendChild(nameIndex);
                        nameIndex.setTextContent(Integer.toString(((ConstantNameAndType)constant).getNameIndex()));
                        
                        Element descriptorIndex = doc.createElement("descriptor_index");
                        element.appendChild(descriptorIndex);
                        descriptorIndex.setTextContent(Integer.toString(((ConstantNameAndType)constant).getSignatureIndex()));
                        break;
                        
                    case 1: /* Utf8 */
                        element.setTextContent( ((ConstantUtf8)constant).getBytes() );
                        break;
                        
                    case 15: /* Methodhandle */
                        Element kind = doc.createElement("reference_kind");
                        element.appendChild(kind);
                        kind.setTextContent(Integer.toString(((ConstantMethodHandle)constant).getReferenceKind()));
                        
                        Element ref_index = doc.createElement("reference_index");
                        element.appendChild(ref_index);
                        ref_index.setTextContent(Integer.toString(((ConstantMethodHandle)constant).getReferenceIndex()));
                        break;
                        
                    case 16: /* MethodType */
                        Element descriptor = doc.createElement("descriptor_index");
                        element.appendChild(descriptor);
                        descriptor.setTextContent(Integer.toString(((ConstantMethodType)constant).getDescriptorIndex()));
                        break;
                        
                    case 18: /* InvokeDynamic */
                        Element bootstrap = doc.createElement ("bootstrap_method_attr_index");
                        element.appendChild(bootstrap);
                        bootstrap.setTextContent(Integer.toString(((ConstantInvokeDynamic)constant).getBootstrapMethodAttrIndex()));
                        
                        Element nametype = doc.createElement ("bootstrap_method_attr_index");
                        element.appendChild(nametype);
                        nametype.setTextContent(Integer.toString(((ConstantInvokeDynamic)constant).getNameAndTypeIndex()));
                        break;
                        
                    default:
                        this.logger.fatal("Invalid constant pool tag " + constant.getTag());
                        System.exit(1);
                }
            }
            
            String[] subdirs = clazz.getClassName().split("\\.");
            subdirs[subdirs.length - 1] = subdirs[subdirs.length - 1] + ".xml";
            File dest = Paths.get(this.dest_dir.getAbsolutePath(), subdirs).toFile();
            dest.getParentFile().mkdirs();
            this.logger.trace ("Writing data to " + dest.getAbsolutePath());
            
            Transformer transformer = TransformerFactory.newInstance().newTransformer();
            DOMSource source = new DOMSource (doc);
            StreamResult result = new StreamResult (dest);
            transformer.transform(source, result);
        } catch (IOException | ParserConfigurationException | TransformerException ex) {
            this.logger.fatal(ex.getMessage());
            System.exit(1);
        }
    }
    
}
