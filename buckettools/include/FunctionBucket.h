
#ifndef __FUNCTIONBUCKET_H
#define __FUNCTIONBUCKET_H

#include "BoostTypes.h"
#include <dolfin.h>

namespace buckettools
{

  // Predeclaration of parent classes to allow two-way dependencies
  class System;
  
  // The FunctionBucket class describes system functions and coefficients and provides data types
  // to the underlying functionals.
  class FunctionBucket
  {
  // only accessible by this class
  private:

    // Empty the data structures of the function
    void empty_();

  protected:
    
    // the function name
    std::string name_;

    // ufl symbol
    std::string uflsymbol_;

    uint index_;

    uint component_;

    // the system to which this function belongs
    System* system_;

    FunctionSpace_ptr functionspace_;

    // the function
    GenericFunction_ptr function_, oldfunction_, iteratedfunction_;

    // expression for the initial condition (if it exists)
    Expression_ptr icexpression_;

    // a map from functional names to the forms (the real point of this class)
    std::map< std::string, Form_ptr > functionals_;

    // a map from components to the (sub)functionspaces they are described on
    std::map< int, FunctionSpace_ptr > subfunctionspaces_;
    
    // a map from bc names to the Expressions describing them
    std::map< std::string, Expression_ptr > bcexpressions_;
    
    // a map from bc::id names to dirichlet boundary conditions
    std::map< std::string, DirichletBC_ptr > dirichletbcs_;
    
    int size_;

    std::vector< int > shape_;

    // would prefer to switch this to an integer at some point
    std::string rank_;
    
    // would prefer to switch this to an integer at some point
    std::string type_;
    
  public:

    // Default constructor
    FunctionBucket();

    // Specific constructor with an uninitialised name
    FunctionBucket(System* system);
    
    // Default destructor
    ~FunctionBucket();

    // Register a subfunctionspace in the system
    void register_subfunctionspace(FunctionSpace_ptr subfunctionspace, int component);

    // Return whether a subfunctionspace with the given name is in the system
    bool contains_subfunctionspace(int component);

    // Return a pointer to a dolfin subfunctionspace with the given name
    FunctionSpace_ptr fetch_subfunctionspace(int component);

    // Register a functional in the function
    void register_functional(Form_ptr functional, std::string name);

    // Return a pointer to a functional with the given name
    Form_ptr fetch_functional(std::string name);

    // Register a bc expression in the functionbucket
    void register_bcexpression(Expression_ptr bcexpression, std::string name);

    // Register a bc expression in the functionbucket
    void register_dirichletbc(DirichletBC_ptr bc, std::string name);

    // Return a string describing the contents of the function
    virtual std::string str() const
    { str(0); }

    // Return a string describing the contents of the function
    virtual std::string str(int indent) const;

    // Print a description of the functionals contained in the function
    virtual std::string functionals_str() const
    { functionals_str(0); }

    // Print a description of the functionals contained in the system
    virtual std::string functionals_str(int indent) const;

    // Return the function name
    std::string name() const
    { return name_; }

    // Return the function uflsymbol
    std::string uflsymbol() const
    { return uflsymbol_; }

    // Return the function_
    FunctionSpace_ptr functionspace() const
    { return functionspace_; }

    // Return the function_
    GenericFunction_ptr function() const
    { return function_; }

    // Return the oldfunction_
    GenericFunction_ptr oldfunction() const
    { return oldfunction_; }

    // Return the iteratedfunction_
    GenericFunction_ptr iteratedfunction() const
    { return iteratedfunction_; }

    // Return the icexpression_
    Expression_ptr icexpression() const
    { return icexpression_; }

  };

  // Define a boost shared pointer to the system class type
  typedef boost::shared_ptr< FunctionBucket > FunctionBucket_ptr;

}
#endif
