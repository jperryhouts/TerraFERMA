
#ifndef __BUCKET_H
#define __BUCKET_H

// #include "GenericDetectors.h"
// #include "PythonDetectors.h"
#include "DolfinBoostTypes.h"
#include "System.h"
#include <dolfin.h>

namespace buckettools
{
  
  // Some iterators
  typedef std::map< std::string, System_ptr >::iterator                 System_it;
  typedef std::map< std::string, System_ptr >::const_iterator           System_const_it;
  typedef std::map< std::string, Mesh_ptr >::iterator                   Mesh_it;
  typedef std::map< std::string, Mesh_ptr >::const_iterator             Mesh_const_it;
  typedef std::map< std::string, std::string >::iterator                string_it;
  typedef std::map< std::string, std::string >::const_iterator          string_const_it;
  // typedef std::map< std::string, GenericDetectors_ptr >::iterator       GenericDetectors_it;
  // typedef std::map< std::string, GenericDetectors_ptr >::const_iterator GenericDetectors_const_it;
  
  class Bucket
  {
  private:

    std::string name_;
    
    void empty_();

  protected:    

    std::map< std::string, Mesh_ptr >         meshes_;

    std::map< std::string, System_ptr >       systems_;

    //std::map< std::string, GenericDetectors_ptr >    detectors_;
    //std::map< std::string, std::string >      detector_optionpaths_;
    
  public:
    
    Bucket()
    { Bucket("uninitialised_name"); }

    Bucket(std::string name);
    
    ~Bucket();

    void register_mesh(Mesh_ptr mesh, std::string name);

    std::string str() const;
    
    virtual std::string meshes_str() const;

    std::string name() const
    { return name_; }
    
    //void register_system(System_ptr std::string name)
    //{ register_system(name, "uninitialised_path"); }

    //void register_system(std::string name, std::string option_path);

    //void register_detector(GenericDetectors_ptr detector, std::string name)
    //{ register_detector(detector, name, "uninitialised_path"); }
    //
    //void register_detector(GenericDetectors_ptr detector, std::string name, std::string option_path);
    //
    //void register_functionspace(FunctionSpace_ptr functionspace, std::string systemname, std::string name);
    //
    //void register_dirichletbc(DirichletBC_ptr dirichletbc, std::string systemname, std::string name);
    //
    //void register_function(Function_ptr function, std::string systemname, std::string name)
    //{ register_function(function, systemname, name, "uninitialised_path"); }
    //
    //void register_function(Function_ptr function, std::string systemname, std::string name, std::string option_path);
    
    Mesh_ptr fetch_mesh(const std::string name);
    
    //FunctionSpace_ptr fetch_functionspace(const std::string systemname, const std::string name);
    //
    //Function_ptr fetch_function(const std::string systemname, const std::string name);
    //
    //GenericDetectors_ptr fetch_detector(const std::string name);
    //

    Mesh_it meshes_begin();

    Mesh_const_it meshes_begin() const;

    Mesh_it meshes_end();

    Mesh_const_it meshes_end() const;
 
    //std::map< std::string, GenericDetectors_ptr >::iterator detectors_begin();
    //
    //std::map< std::string, GenericDetectors_ptr >::const_iterator detectors_begin() const;
    //
    //std::map< std::string, GenericDetectors_ptr >::iterator detectors_end();
    //
    //std::map< std::string, GenericDetectors_ptr >::const_iterator detectors_end() const;
    //
    //std::map< std::string, DirichletBC_ptr >::iterator dirichletbcs_begin();
    //
    //std::map< std::string, DirichletBC_ptr >::const_iterator dirichletbcs_begin() const;
    //
    //std::map< std::string, DirichletBC_ptr >::iterator dirichletbcs_end();
    //
    //std::map< std::string, DirichletBC_ptr >::const_iterator dirichletbcs_end() const;
    //
    //std::map< std::string, Function_ptr >::iterator functions_begin();
    //
    //std::map< std::string, Function_ptr >::const_iterator functions_begin() const;
    //
    //std::map< std::string, Function_ptr >::iterator functions_end();
    //
    //std::map< std::string, Function_ptr >::const_iterator functions_end() const;
    
  protected:
    
    //void register_bcexp(GenericFunction_ptr bcexp, std::string systemname);
    
  };

  typedef boost::shared_ptr< Bucket > Bucket_ptr;

}
#endif
