package com.shilling.datagen;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Enumeration;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 *
 * @author Jake Shilling <shilling.jake@gmail.com>
 */
public class Datagen {

    private static final Logger logger
            = LogManager.getLogger(Datagen.class);

    private static final Executor exec =
            Executors.newFixedThreadPool(100);

    public static void main(String[] args) {
        logger.info("Looking for JRE home");
        String java_home = System.getProperty("java.home");
        logger.trace("JAVA_HOME = " + java_home);

        File javaHome = new File(java_home);
        if (!javaHome.exists()) {
            logger.fatal("JAVA_HOME (" + java_home + ") does not exist.");
            System.exit(1);
        }

        if (!javaHome.isDirectory()) {
            logger.fatal("JAVA_HOME (" + java_home + ") is not a directory");
            System.exit(1);
        }

        logger.trace("Checking whether JAVA_HOME is a JRE or JDK");

        File jre = Paths.get(java_home, "jre").toFile();
        if (jre.exists()) {
            logger.trace("Found JRE within JDK");
        } else {
            logger.trace("JAVA_HOME is a JRE");
            jre = javaHome;
        }

        File rt = Paths.get(jre.getAbsolutePath(), "lib", "rt.jar").toFile();
        if (!rt.exists()) {
            logger.fatal("File not found: " + rt.getAbsolutePath());
            System.exit(1);
        }

        if (!rt.isFile()) {
            logger.fatal(rt.getAbsoluteFile() + " is not a file");
            System.exit(1);
        }

        Path out;
        if (args.length > 0) {
            out = Paths.get(args[0]);
        } else {
            out = Paths.get(System.getProperty("user.dir"));
        }

        File dest = Paths.get(out.toString(), "rt").toFile();
        logger.trace("Extracting rt.jar to " + dest.getAbsolutePath());
        
        try {

            JarFile jar = new JarFile(rt);
            Enumeration entries = jar.entries();
            while (entries.hasMoreElements()) {
                JarEntry entry = (JarEntry) entries.nextElement();
                exec.execute(new WriteJarEntry (exec, dest, entry, jar));
            }

        } catch (IOException e) {

            logger.fatal("Error extracting rt.jar: " + e.getMessage());
            System.exit(1);

        }
    }
}
