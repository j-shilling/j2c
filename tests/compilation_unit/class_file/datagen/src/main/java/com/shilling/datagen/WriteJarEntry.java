/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.shilling.datagen;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Paths;
import java.util.concurrent.Executor;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 *
 * @author Jake Shilling <shilling.jake@gmail.com>
 */
public class WriteJarEntry implements Runnable {
    
    private final Logger logger =
            LogManager.getLogger(WriteJarEntry.class);
    
    private final Executor exec;
    
    private final File dest;
    private final File data_dir;
    private final JarEntry src;
    private final JarFile jar;
    
    public WriteJarEntry (Executor exec, File dest, JarEntry src, JarFile jar) {
        this.exec = exec;
        this.dest = dest;
        this.src = src;
        this.jar = jar;
        
        this.data_dir = Paths.get(this.dest.getParentFile().getAbsolutePath(),
                "test_data").toFile();
    }

    @Override
    public void run() {
        if (this.src.isDirectory())
            return;
        if (!this.src.getName().endsWith(".class"))
            return;
        
        this.logger.trace("Extracting " + src.getName());
        
        File file = Paths.get(dest.getPath(), this.src.getName()).toFile();
        file.getParentFile().mkdirs();
        
        try (FileOutputStream fos = new FileOutputStream (file);
             InputStream in = this.jar.getInputStream(this.src)) {
            
            while (in.available() > 0)
                fos.write(in.read());
            this.exec.execute(new ParseJarEntry(this.data_dir, file));
            
        } catch (FileNotFoundException ex) {
            this.logger.fatal(ex.getMessage());
            System.exit(1);
        } catch (IOException ex) {
            this.logger.fatal(ex.getMessage());
            System.exit(1);
        }
    }
    
}
