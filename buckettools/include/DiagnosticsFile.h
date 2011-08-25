
#ifndef __DIAGNOSTICS_FILE_H
#define __DIAGNOSTICS_FILE_H

#include <cstdio>
#include <fstream>
#include <string>
#include "StatFile.h"
#include "Bucket.h"

namespace buckettools
{

  //*****************************************************************|************************************************************//
  // DiagnosticsFile class:
  //
  // A derived class from the base statfile class intended for the output of diagnostics to file every dump period.
  // Statistics normally include things like function mins and maxes as well as functional output.
  //*****************************************************************|************************************************************//
  class DiagnosticsFile : public StatFile
  {

  //*****************************************************************|***********************************************************//
  // Publicly available functions
  //*****************************************************************|***********************************************************//

  public:                                                            // available to everyone
    
    //***************************************************************|***********************************************************//
    // Constructors and destructors
    //***************************************************************|***********************************************************//
    
    DiagnosticsFile(const std::string name);                         // optional constructor
 
    ~DiagnosticsFile();                                              // default destructor
    
    //***************************************************************|***********************************************************//
    // Header writing functions
    //***************************************************************|***********************************************************//

    void write_header(const Bucket &bucket,                          // write header for the bucket, indicating if this is a dynamic
                      const bool &timestepping);                     // simulation or not

    //***************************************************************|***********************************************************//
    // Data writing functions
    //***************************************************************|***********************************************************//

    void write_data(Bucket &bucket);                                 // write data to file for a steady state simulation
    
    void write_data(const uint   &timestep,                          // write data to file for a dynamics simulation
                    const double &elapsedtime, 
                    const double &dt, 
                    Bucket       &bucket);
    
  private:                                                           // only available to this class

    //***************************************************************|***********************************************************//
    // Header writing functions (continued)
    //***************************************************************|***********************************************************//

    void header_bucket_(const Bucket &bucket,                        // write the header for the bucket (non-constant and 
                        uint &column);                               // timestepping entries)

    void header_system_(SystemBucket_const_it sys_it,                // write the header for a system
                        uint &column);

    void header_field_(FunctionBucket_const_it f_begin,              // write the header for a set of fields
                       FunctionBucket_const_it f_end, 
                       uint &column);

    void header_coeff_(FunctionBucket_const_it f_begin,              // write the header for a set of coefficients
                       FunctionBucket_const_it f_end, 
                       uint &column);

    void header_functional_(FunctionBucket_const_it f_it,            // write the header for a set of functionals of a function
                            Form_const_it s_begin,
                            Form_const_it s_end, 
                            uint &column);

    //***************************************************************|***********************************************************//
    // Data writing functions (continued)
    //***************************************************************|***********************************************************//

    void data_timestep_(const uint   &timestep,                      // write the data for a dynamic simulation
                        const double &elapsedtime, 
                        const double &dt);
    
    void data_bucket_(Bucket &bucket);                               // write the data for a steady state simulation

    void data_system_(SystemBucket_const_it sys_it);                 // write the data for a system

    void data_field_(FunctionBucket_const_it f_begin,                // write the data for a set of fields
                     FunctionBucket_const_it f_end);

    void data_coeff_(FunctionBucket_const_it f_begin,                // write the data for a set of coefficients
                     FunctionBucket_const_it f_end);

    void data_functional_(Form_const_it s_begin,                     // write the data for a set of functionals
                          Form_const_it s_end);

  };
  
}
#endif
