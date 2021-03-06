package org.kakooge.dacho.tests;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

public class TestRunner {
   public static void main(String[] args) {
	  System.out.print("Running tests...");
      Result result = JUnitCore.runClasses(ServiceBootstrapTest.class
    		  ,ServiceClassloaderTest.class
    		  ,IOUtilsTest.class
    		  );
      
      for (Failure failure : result.getFailures()) {
         System.out.println(failure.toString());
      }
      System.out.print((result.wasSuccessful() ? "Pass\n" : "Fail\n"));
   }
} 
