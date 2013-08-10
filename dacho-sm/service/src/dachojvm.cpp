#include <string.h>
#include "dachojvm.h"
#include <stdexcept>

void DachoJVM::init(){

	// Make options
  std::map<std::string, std::string>::iterator m_it = m_properties.begin();
  std::vector<std::string>::iterator v_it = v_optionstr.begin();
  std::vector<JavaVMOption>::iterator vo_it = v_options.begin();
  for (; m_it!=m_properties.end(); ++m_it, ++v_it, ++vo_it){
	  v_it->append(m_it->first);
	  if(m_it->second.length()>0){
		  v_it->append("=").append(m_it->second);
	  }
	  vo_it->optionString = const_cast<char *>(v_it->c_str());
  }

  vm_args.version  = JNI_VERSION_1_6;                   /* Specifies the JNI version used */
  vm_args.options  = &v_options[0];
  vm_args.nOptions = v_options.size();
  vm_args.ignoreUnrecognized = JNI_TRUE;                 /* JNI won't complain about unrecognized options */

  //- Start machine
  if (JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args)) 
	  throw std::runtime_error("Failed to create the JVM");

  if(env->GetVersion() < SUPPORTED_JVM)
    throw std::runtime_error("Unsupported JVM version, please upgrade");
}

void DachoJVM::destroy(){
  if(jvm)
    jvm->DestroyJavaVM(); /* kill the JVM */
}

void DachoJVM::executeMethod(const std::string &className, const std::string &methodName, const std::string &signature){

  if(!env)
	  throw std::runtime_error("JVM Environemnt has not been initialized"); /* error */

  jclass cls = env->FindClass(className.c_str());

  /* find the main() method */
  jmethodID methodId = env->GetStaticMethodID(cls, methodName.c_str(), signature.c_str());
	
  if( methodId == 0 )
    throw std::runtime_error("Could not find method. Failed to execute method"); /* error */
	
  env->CallStaticVoidMethod(cls, methodId, 0); /* call method() */
}

DachoJVM::DachoJVM(std::map<std::string, std::string> &properties)
: m_properties(properties), v_options(properties.size()), v_optionstr(properties.size()){}
DachoJVM::~DachoJVM() throw(){}
/*
DachoJVM::DachoJVM(const DachoJVM &other) : properties(other.properties){}
DachoJVM & DachoJVM::operator=(const DachoJVM &other){}
*/