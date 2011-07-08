
#include "SpudBucket.h"
#include "SpudSystem.h"
#include "DolfinBoostTypes.h"
//#include "GenericDetectors.h"
//#include "PointDetectors.h"
//#include "PythonDetectors.h"
//#include "PythonExpression.h"
//#include "InitialConditionExpression.h"
#include <dolfin.h>
#include <string>
#include <spud.h>

using namespace buckettools;

// Default constructor for spudbucket derived class
SpudBucket::SpudBucket(std::string name, std::string optionpath) : optionpath_(optionpath), Bucket(name)
{
  // Do nothing
}

// Default destructor for spudbucket derived class
SpudBucket::~SpudBucket()
{
  // Do nothing
}

// Fill the bucket with stuff based on options tree provided by Spud
void SpudBucket::fill()
{
  // A buffer to put option paths in
  std::stringstream buffer;
  
  // Put meshes into the bucket
  buffer.str(""); buffer << "/geometry/mesh";  
  int nmeshes = Spud::option_count(buffer.str());
  for (uint i = 0; i<nmeshes; i++)
  {
    buffer << "[" << i << "]";
    meshes_fill_(buffer.str());
  }
  
  // Put systems into the bucket
  buffer.str(""); buffer << "/system";
  int nsystems = Spud::option_count(buffer.str());
  for (uint i = 0; i<nsystems; i++)
  {
    buffer << "[" << i << "]";
    systems_fill_(buffer.str());
  }
  
  //// Put detectors in the bucket
  //detectors_fill_();

}

// Insert a mesh (with given optionpath) into the bucket
void SpudBucket::meshes_fill_(const std::string &optionpath)
{
  // A buffer to put option paths in
  std::stringstream buffer;
  
  // Get the name of the mesh
  std::string meshname;
  buffer.str(""); buffer << optionpath << "/name";
  Spud::get_option(buffer.str(), meshname);
  
  // Get the name of the mesh file
  std::string basename;
  buffer.str(""); buffer << optionpath << "/file";
  Spud::get_option(buffer.str(), basename);
  
  // Use DOLFIN to read in the mesh
  std::stringstream filename;
  filename.str(""); filename << basename << ".xml";
  Mesh_ptr mesh(new dolfin::Mesh(filename.str()));
  (*mesh).init();

  // Register the mesh functions (in dolfin::MeshData associated with the mesh - saves us having a separate set of mesh functions in
  // the Bucket, which would need to be associated with a particular mesh in the case of multiple meshes!):
  // - for the edge ids
  MeshFunction_uint_ptr edgeids = (*mesh).data().create_mesh_function("EdgeIDs");
  filename.str(""); filename << basename << "_edge_subdomain.xml";
  edgeids.reset(new dolfin::MeshFunction<dolfin::uint>(*mesh, filename.str()));

  // - for the cell ids
  MeshFunction_uint_ptr cellids = (*mesh).data().create_mesh_function("CellIDs");
  filename.str(""); filename << basename << "_cell_subdomain.xml";
  cellids.reset(new dolfin::MeshFunction<dolfin::uint>(*mesh, filename.str()));

  // Put the mesh into the bucket
  register_mesh(mesh, meshname, optionpath);
}

void SpudBucket::register_mesh(Mesh_ptr mesh, std::string name, std::string optionpath)
{
  // First check if a mesh with this name already exists
  Mesh_it m_it = meshes_.find(name);
  if (m_it != meshes_.end())
  {
    // if it does, issue an error
    dolfin::error("Mesh named \"%s\" already exists in spudbucket.", name.c_str());
  }
  else
  {
    // if not then insert it into the maps
    meshes_[name]           = mesh;
    mesh_optionpaths_[name] = optionpath;
  }
}

std::string SpudBucket::fetch_mesh_optionpath(const std::string name)
{
  // Check if the mesh exists in the bucket
  string_it s_it = mesh_optionpaths_.find(name);
  if (s_it == mesh_optionpaths_.end())
  {
    // if it doesn't then throw an error
    dolfin::error("Mesh named \"%s\" does not exist in spudbucket.", name.c_str());
  }
  else
  {
    // if it does then return the pointer to the mesh
    return (*s_it).second;
  }
}

std::string SpudBucket::meshes_str() const
{
  std::stringstream s;

  for ( string_const_it s_it = mesh_optionpaths_begin(); s_it != mesh_optionpaths_end(); s_it++ )
  {
    s << "Mesh " << (*s_it).first << ": " << (*s_it).second  << std::endl;
  }

  return s.str();
}

string_it SpudBucket::mesh_optionpaths_begin()
{
  return mesh_optionpaths_.begin();
}

string_const_it SpudBucket::mesh_optionpaths_begin() const
{
  return mesh_optionpaths_.begin();
}

string_it SpudBucket::mesh_optionpaths_end()
{
  return mesh_optionpaths_.end();
}

string_const_it SpudBucket::mesh_optionpaths_end() const
{
  return mesh_optionpaths_.end();
}
// Insert a system (with optionpath) into the bucket
void SpudBucket::systems_fill_(const std::string &optionpath)
{
  // A string buffer for option paths
  std::stringstream buffer;

  // Get the system name
  std::string sysname;
  buffer.str(""); buffer << optionpath << "/name";
  Spud::get_option(buffer.str(), sysname);

  // Get the name of the mesh this system is defined on
  std::string meshname;
  buffer.str(""); buffer << optionpath << "/mesh/name";
  Spud::get_option(buffer.str(), meshname);
  // and then extract the mesh from the bucket we're filling
  Mesh_ptr mesh = fetch_mesh(meshname);

  SpudSystem_ptr system( new SpudSystem(sysname, optionpath, mesh) );

  (*system).fill();
//  
//  // Get the dimension of the problem (only one dimension assumed currently)
//  int dimension;
//  Spud::get_option("/geometry/dimension", dimension);
//  
}
//
//void SpudBucket::bc_fill_(const std::string bcpath, 
//                          const int funci, 
//                          const int dimension, 
//                          FunctionSpace_ptr subsysspace,
//                          const std::string sysname,
//                          const std::string funcname)
//{
//  std::stringstream buffer;
//  
//  MeshFunction_uint_ptr edge_subdomain = fetch_meshfunction("EdgeIDs");
//  
//  std::string bcname;
//  buffer.str(""); buffer << bcpath << "/name";
//  Spud::get_option(buffer.str(), bcname);
//  
//  std::vector<int> bcids;
//  buffer.str(""); buffer << bcpath << "/boundary_ids";
//  Spud::get_option(buffer.str(), bcids);
//  
//  buffer.str(""); buffer << bcpath << "/sub_components";
//  int nsubcomp = Spud::option_count(buffer.str());
//  for (uint subcompi = 0; subcompi < nsubcomp; subcompi++)
//  {
//    std::stringstream subcompbuffer;
//    subcompbuffer.str(""); subcompbuffer << bcpath << "/sub_components[" << subcompi << "]";
//    
//    std::vector<int> subcompids;
//    buffer.str(""); buffer << subcompbuffer.str() << "/components";
//    if(Spud::have_option(buffer.str()))
//    {
//      Spud::get_option(buffer.str(), subcompids);
//      
//      for (std::vector<int>::iterator subcompid = subcompids.begin(); subcompid < subcompids.end(); subcompid++)
//      {
//        
//        FunctionSpace_ptr subsubsysspace(new dolfin::SubSpace(*subsysspace, *subcompid));
//        buffer.str(""); buffer << sysname << "::" << funcname << "::Space_" << *subcompid;
//        register_functionspace(subsubsysspace, buffer.str());
//        
//        buffer.str(""); buffer << subcompbuffer.str() << "/type::Dirichlet";
//        GenericFunction_ptr bcexp = init_exp_(buffer.str(), dimension);
//        
//        register_bcexp(bcexp);
//        
//        for (std::vector<int>::iterator bcid = bcids.begin(); bcid < bcids.end(); bcid++)
//        {
//          DirichletBC_ptr bc(new dolfin::DirichletBC(*subsubsysspace, *bcexp, *edge_subdomain, *bcid));
//          buffer.str(""); buffer << bcname << "_" << *bcid << "_" << *subcompid;
//          register_dirichletbc(bc, buffer.str());
//        }
//        
//      }
//    }
//    else
//    {
//      buffer.str(""); buffer << subcompbuffer.str() << "/type::Dirichlet";
//      GenericFunction_ptr bcexp = init_exp_(buffer.str(), dimension);
//      
//      register_bcexp(bcexp);
//      
//      for (std::vector<int>::iterator bcid = bcids.begin(); bcid < bcids.end(); bcid++)
//      {
//        DirichletBC_ptr bc(new dolfin::DirichletBC(*subsysspace, *bcexp, *edge_subdomain, *bcid));
//        buffer.str(""); buffer << bcname << "_" << *bcid;
//        register_dirichletbc(bc, buffer.str());
//      }
//    }
//  }
//}
//
//GenericFunction_ptr SpudBucket::init_exp_(const std::string path, const int dimension)
//{
//  GenericFunction_ptr bcexp(new dolfin::Expression());
//  
//  std::stringstream constbuffer, pybuffer;
//  constbuffer.str(""); constbuffer << path << "/constant";
//  pybuffer.str(""); pybuffer << path << "/python";
//  
//  if(Spud::have_option(constbuffer.str()))
//  {
//    int rank;
//    Spud::get_option_rank(constbuffer.str(), rank);
//    if(rank==0)
//    {
//      double value;
//      Spud::get_option(constbuffer.str(), value);
//      bcexp.reset(new dolfin::Constant(value));
//    }
//    else if (rank==1)
//    {
//      std::vector<double> values;
//      Spud::get_option(constbuffer.str(), values);
//      bcexp.reset(new dolfin::Constant(values));
//    }
//    else
//    {
//      dolfin::error("Don't know how to deal with rank > 1 yet.");
//    }
//  } 
//  else if(Spud::have_option(pybuffer.str()))
//  {
//    std::string pyfunction;
//    Spud::get_option(pybuffer.str(), pyfunction);
//    
//    // rank of a python function isn't in the default spud base language
//    // so have added it... but it comes out as a string of course!
//    std::stringstream buffer;
//    std::string rankstring; // bit of a hack
//    buffer.str(""); buffer << pybuffer.str() << "/rank";
//    Spud::get_option(buffer.str(), rankstring);
//    
//    int rank;
//    rank = atoi(rankstring.c_str());
//    if(rank==0)
//    {
//      bcexp.reset(new buckettools::PythonExpression(pyfunction));
//    }
//    else if (rank==1)
//    {
//      bcexp.reset(new buckettools::PythonExpression(dimension, pyfunction));
//    }
//    else
//    {
//      dolfin::error("Don't know how to deal with rank > 1 yet.");
//    }
//  }
//  else
//  {
//    dolfin::error("Unknown way of specifying bc expression.");
//  }
//  
//  return bcexp;
//  
//}
//
//void SpudBucket::detectors_fill_()
//{
//  // Initialise a pointer to a generic detector - so that it can be reset in the loops
//  GenericDetectors_ptr det(new GenericDetectors());
//  // A string buffer for option paths
//  std::stringstream buffer;
//  
//  // Find out how many point detectors there are
//  int npdets = Spud::option_count("/io/detectors/point");
//  // and loop over them
//  for (uint i=0; i<npdets; i++)
//  {
//    // Set up the base path for a point detector
//    std::stringstream detectorpath;
//    detectorpath.str(""); detectorpath << "/io/detectors/point[" << i << "]";
//    
//    // Get the name of the detector
//    std::string detname;
//    buffer.str(detectorpath.str()); buffer << "/name";
//    Spud::get_option(buffer.str(), detname);
//
//    // Get the location of the detector
//    std::vector<double> point;
//    buffer.str(detectorpath.str());
//    Spud::get_option(buffer.str(), point);
//    
//    // Initialise and register the point detector
//    det.reset(new PointDetectors(&point, detname));
//    register_detector(det, detname);
//  }
//  
//  // Find out how many detector arrays there are
//  int nadets = Spud::option_count("/io/detectors/array");
//  // and loop over them
//  for (uint i=0; i<nadets; i++)
//  {
//    // Set up the base path for a detector array
//    std::stringstream detectorpath;
//    detectorpath.str(""); detectorpath << "/io/detectors/array[" << i << "]";
//    
//    // Get the number of points in this array
//    int no_det;
//    buffer.str(detectorpath.str()); buffer << "/number_of_detectors";
//    Spud::get_option(buffer.str(), no_det);
//
//    // Get the detector array name
//    std::string detname;
//    buffer.str(detectorpath.str()); buffer << "/name";
//    Spud::get_option(buffer.str(), detname);
//
//    // Get the python function that describes the positions of the detectors
//    std::string function;
//    buffer.str(detectorpath.str()); buffer << "/python";
//    Spud::get_option(buffer.str(), function);
//    
//    // Get the dimension of this problem
//    // - here we assume that there is only one dimension in this problem
//    int dimension;
//    Spud::get_option("/geometry/dimension", dimension);
//
//    // Create the detector and register it in the bucket
//    det.reset(new PythonDetectors(no_det, dimension, function, detname));
//    register_detector(det, detname);
//  }  
//  
//}
