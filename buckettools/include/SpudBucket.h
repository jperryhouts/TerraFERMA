
#ifndef __SPUD_BUCKET_H
#define __SPUD_BUCKET_H

#include "Bucket.h"
#include "BoostTypes.h"
#include <dolfin.h>

namespace buckettools
{

  //*****************************************************************|************************************************************//
  // SpudBucket class:
  //
  // The SpudBucket derived class uses the data structures of the Bucket class,
  // which describes a numerical model, and populates them (plus some supplementary
  // material) using the spud options parser.
  //*****************************************************************|************************************************************//
  class SpudBucket : public Bucket
  {

  //*****************************************************************|***********************************************************//
  // Publicly available functions
  //*****************************************************************|***********************************************************//

  public:                                                            // accessible to everyone
    
    //***************************************************************|***********************************************************//
    // Constructors and destructors
    //***************************************************************|***********************************************************//

    SpudBucket();                                                    // Default constructor
    
    SpudBucket(const std::string &name);                             // Specific constructor (specifying name)
    
    SpudBucket(const std::string &name, 
                          const std::string &optionpath);            // Specific constructor (specifying name and optionpath)

    ~SpudBucket();                                                   // Default destructor
    
    //***************************************************************|***********************************************************//
    // Filling data
    //***************************************************************|***********************************************************//

    void fill();                                                     // fill the bucket data structures assuming the buckettools
                                                                     // spud schema

    void copy_diagnostics(Bucket_ptr &bucket) const;                 // copy the data necessary for the diagnostics data file(s)

    //***************************************************************|***********************************************************//
    // Base data access
    //***************************************************************|***********************************************************//

    const std::string optionpath()                                   // return the optionpath for this bucket
    { return optionpath_; }                                          // (normally an empty string)

    //***************************************************************|***********************************************************//
    // Mesh data access
    //***************************************************************|***********************************************************//

    void register_mesh(Mesh_ptr mesh, const std::string &name,       // register a mesh with a given name (and an optionpath)
                       const std::string &optionpath); 

    std::string fetch_mesh_optionpath(const std::string &name);      // return the optionpath associated with the named mesh

    string_it mesh_optionpaths_begin();                              // return an iterator to the beginning of the mesh optionpaths

    string_const_it mesh_optionpaths_begin() const;                  // return a constant iterator to the beginning of the mesh
                                                                     // optionpaths

    string_it mesh_optionpaths_end();                                // return an iterator to the end of the mesh optionpaths

    string_const_it mesh_optionpaths_end() const;                    // return a constant iterator to the end of the mesh
                                                                     // optionpaths
 
    //***************************************************************|***********************************************************//
    // Detector data access
    //***************************************************************|***********************************************************//

    void register_detector(GenericDetectors_ptr detector,            // register a detector with a given name (and an optionpath)
                           const std::string &name, 
                           const std::string &option_path);
    
    std::string fetch_detector_optionpath(const std::string &name);  // return the optionpath associated with the named detector

    string_it detector_optionpaths_begin();                          // return an iterator to the beginning of the detector optionpaths

    string_const_it detector_optionpaths_begin() const;              // return a constant iterator to the beginning of the detector
                                                                     // optionpaths

    string_it detector_optionpaths_end();                            // return an iterator to the end of the detector optionpaths

    string_const_it detector_optionpaths_end() const;                // return a constant iterator to the end of the detector
                                                                     // optionpaths
 
    //***************************************************************|***********************************************************//
    // Output functions
    //***************************************************************|***********************************************************//

    const std::string meshes_str() const                             // return a string describing the meshes
    { return meshes_str(0); }

    const std::string meshes_str(const int &indent) const;           // return an indented string describing the meshes

    const std::string detectors_str() const                          // return a string describing the meshes
    { return detectors_str(0); }

    const std::string detectors_str(const int &indent) const;        // return an indented string describing the meshes

  //*****************************************************************|***********************************************************//
  // Private functions
  //*****************************************************************|***********************************************************//

  private:                                                           // only accessible to this class
    
    //***************************************************************|***********************************************************//
    // Base data
    //***************************************************************|***********************************************************//

    std::string optionpath_;                                         // the option path associated with the bucket (normally an
                                                                     // empty string)

    //***************************************************************|***********************************************************//
    // Pointers data
    //***************************************************************|***********************************************************//

    std::map< std::string, std::string > mesh_optionpaths_;          // a map from mesh names to spud mesh optionpaths

    std::map< std::string, std::string > detector_optionpaths_;      // a map from detector names to spud detector optionpaths

    //***************************************************************|***********************************************************//
    // Filling data (continued)
    //***************************************************************|***********************************************************//

    void fill_timestepping_();                                       // fill the timestepping data
 
    void fill_output_();                                             // fill the output data
 
    void fill_meshes_(const std::string &optionpath);                // fill in the mesh data structures
    
    void fill_systems_(const std::string &optionpath);               // fill in information about the systems

    void fill_baseuflsymbols_(const std::string &optionpath);        // fill the ufl symbol maps

    void fill_detectors_();                                          // fill the detectors
 
    void fill_diagnostics_();                                        // fill the detectors
 
    //***************************************************************|***********************************************************//
    // Emptying data
    //***************************************************************|***********************************************************//

    void empty_();

  };

  typedef boost::shared_ptr< SpudBucket > SpudBucket_ptr;            // define a boost shared pointer type for this class

}
#endif
