
#include "Bucket.h"
#include "GenericDetectors.h"
#include "PythonDetectors.h"
#include <dolfin.h>
#include <string>

using namespace buckettools;

// Specific constructor
Bucket::Bucket(std::string name, std::string option_path) : name_(name), option_path_(option_path)
{
  // Do nothing
}

// Default destructor
Bucket::~Bucket()
{
  // Do nothing
}

// Register a dolfin mesh
void Bucket::register_mesh(Mesh_ptr mesh, std::string name, std::string option_path)
{
  // First check if a mesh with this name already exists
  Mesh_it m_it = meshes_.find(name);
  if (m_it != meshes_.end())
  {
    // if it does, issue an error
    dolfin::error("Mesh named \"%s\" already exists in bucket.", name.c_str());
  }
  else
  {
    // if not then insert it into the maps
    meshes_[name] = mesh;
    mesh_optionpaths_[name] = option_path;
  }
}

// Create a system bucket in this bucket
void Bucket::create_system(std::string name, std::string option_path)
{
  // First check if a system with this name already exists
  SystemBucket_it s_it = systembuckets_.find(name);
  if(s_it != systembuckets_.end())
  {
    // if it does, issue an error
    dolfin::error("System named \"%s\" already exists in bucket.", name.c_str());
  }
  else
  {
    // if not then create it and insert it into the maps
    SystemBucket_ptr system(new SystemBucket(name, option_path));
    systembuckets_[name] = system;
    systembucket_optionpaths_[name] = option_path;
  }
}

// Register a detector set in the bucket
void Bucket::register_detector(GenericDetectors_ptr detector, std::string name, std::string option_path)
{
  // First check if a detector set with this name already exists
  GenericDetectors_it d_it = detectors_.find(name);
  if (d_it != detectors_.end())
  {
    // if it does, issue an error
    dolfin::error("Detector set named \"%s\" already exists in bucket.", name.c_str());
  }
  else
  {
    // if not then insert it into the maps
    detectors_[name] = detector;
    detector_optionpaths_[name] = option_path;
  }
}

void Bucket::register_functionspace(FunctionSpace_ptr functionspace, std::string name)
{
  functionspaces_.insert(std::pair<std::string, FunctionSpace_ptr>(name, functionspace));
}

void Bucket::register_dirichletbc(DirichletBC_ptr dirichletbc, std::string name)
{
  dirichletbcs_.insert(std::pair<std::string, DirichletBC_ptr>(name, dirichletbc));
}

void Bucket::register_function(Function_ptr function, std::string name)
{
  functions_.insert(std::pair<std::string, Function_ptr>(name, function));
}

void Bucket::register_bcexp(GenericFunction_ptr bcexp)
{
  bcexps_.push_back(bcexp);
}

Mesh_ptr Bucket::fetch_mesh(const std::string name)
{
  // Check if the mesh exists in the bucket
  Mesh_it m_it = meshes_.find(name);
  if (m_it == meshes_.end())
  {
    // if it doesn't then throw an error
    dolfin::error("Mesh named \"%s\" does not exist in bucket.", name.c_str());
  }
  else
  {
    // if it does then return the pointer to the mesh
    return (*m_it).second;
  }
}

std::string Bucket::fetch_mesh_optionpath(const std::string name)
{
  // Check if the mesh exists in the bucket
  string_it s_it = mesh_optionpaths_.find(name);
  if (s_it == mesh_optionpaths_.end())
  {
    // if it doesn't then throw an error
    dolfin::error("Mesh named \"%s\" does not exist in bucket.", name.c_str());
  }
  else
  {
    // if it does then return the pointer to the mesh
    return (*s_it).second;
  }
}

FunctionSpace_ptr Bucket::fetch_functionspace(const std::string name)
{
  std::map< std::string, FunctionSpace_ptr >::iterator it;
  it = functionspaces_.find(name);
  return (*it).second;
}

GenericDetectors_ptr Bucket::fetch_detector(const std::string name)
{
  std::map< std::string, GenericDetectors_ptr >::iterator it;
  it = detectors_.find(name);
  return (*it).second;
}

Function_ptr Bucket::fetch_function(const std::string name)
{
  std::map< std::string, Function_ptr >::iterator it;
  it = functions_.find(name);
  return (*it).second;
}

std::map< std::string, GenericDetectors_ptr >::iterator Bucket::detectors_begin()
{
  return detectors_.begin();
}

std::map< std::string, GenericDetectors_ptr >::const_iterator Bucket::detectors_begin() const
{
  return detectors_.begin();
}

std::map< std::string, GenericDetectors_ptr >::iterator Bucket::detectors_end()
{
  return detectors_.end();
}

std::map< std::string, GenericDetectors_ptr >::const_iterator Bucket::detectors_end() const
{
  return detectors_.end();
}

std::map< std::string, DirichletBC_ptr >::iterator Bucket::dirichletbcs_begin()
{
  return dirichletbcs_.begin();
}

std::map< std::string, DirichletBC_ptr >::const_iterator Bucket::dirichletbcs_begin() const
{
  return dirichletbcs_.begin();
}

std::map< std::string, DirichletBC_ptr >::iterator Bucket::dirichletbcs_end()
{
  return dirichletbcs_.end();
}

std::map< std::string, DirichletBC_ptr >::const_iterator Bucket::dirichletbcs_end() const
{
  return dirichletbcs_.end();
}

std::map< std::string, Function_ptr >::iterator Bucket::functions_begin()
{
  return functions_.begin();
}

std::map< std::string, Function_ptr >::const_iterator Bucket::functions_begin() const
{
  return functions_.begin();
}

std::map< std::string, Function_ptr >::iterator Bucket::functions_end()
{
  return functions_.end();
}

std::map< std::string, Function_ptr >::const_iterator Bucket::functions_end() const
{
  return functions_.end();
}

void Bucket::empty_()
{
  
  meshes_.clear();
  detectors_.clear();
  functions_.clear();
  
}