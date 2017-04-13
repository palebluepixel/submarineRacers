/*! \file cs237-shader.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline functions for shader operations.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _SHADER_INL_
#define _SHADER_INL_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

  //! set the specified int uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the integer value bound to the variable
    inline void setUniform (int loc, int const &v)
    {
      glUniform1i (loc, v);
    }

  //! set the specified int2 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2)
    {
      glUniform2i (loc, v1, v2);
    }

  //! set the specified int3 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2, int const &v3)
    {
      glUniform3i (loc, v1, v2, v3);
    }

  //! set the specified int4 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
  //! \param v4 the fourth component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2, int const &v3, int const &v4)
    {
      glUniform4i (loc, v1, v2, v3, v4);
    }

  //! set the specified unsigned int uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the unsigned integer value bound to the variable
    inline void setUniform (int loc, unsigned int const &v)
    {
      glUniform1ui (loc, v);
    }

  //! set the specified uint2 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2)
    {
      glUniform2ui (loc, v1, v2);
    }

  //! set the specified uint3 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3)
    {
      glUniform3ui (loc, v1, v2, v3);
    }

  //! set the specified uint4 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
  //! \param v4 the fourth component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3, unsigned int const &v4)
    {
      glUniform4ui (loc, v1, v2, v3, v4);
    }

  //! set the specified float uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the float value bound to the variable
    inline void setUniform (int loc, float const &v)
    {
      glUniform1f (loc, v);
    }

    inline void setUniform (int loc, vec2 const &v)
    {
      glUniform2fv (loc, 1, value_ptr(v));
    }

    inline void setUniform (int loc, vec3 const &v)
    {
      glUniform3fv (loc, 1, value_ptr(v));
    }

    inline void setUniform (int loc, vec4 const &v)
    {
      glUniform4fv (loc, 1, value_ptr(v));
    }

    inline void setUniform (int loc, mat2 const &v)
    {
      glUniformMatrix2fv (loc, 1, GL_FALSE, value_ptr(v));
    }

    inline void setUniform (int loc, mat3 const &v)
    {
      glUniformMatrix3fv (loc, 1, GL_FALSE, value_ptr(v));
    }

    inline void setUniform (int loc, mat4 const &v)
    {
      glUniformMatrix4fv (loc, 1, GL_FALSE, value_ptr(v));
    }


  /***** functions to set individual attribute variables *****/

    inline void setAttribute (int loc, int const &v)
    {
      glVertexAttribI1i (loc, v);
    }

    inline void setAttribute (int loc, unsigned int const &v)
    {
      glVertexAttribI1ui (loc, v);
    }

    inline void setAttribute (int loc, float const &v)
    {
      glVertexAttrib1f (loc, v);
    }

    inline void setAttribute (int loc, vec2 const &v)
    {
      glVertexAttrib2fv (loc, value_ptr(v));
    }

    inline void setAttribute (int loc, vec3 const &v)
    {
      glVertexAttrib3fv (loc, value_ptr(v));
    }

    inline void setAttribute (int loc, vec4 const &v)
    {
      glVertexAttrib4fv (loc, value_ptr(v));
    }

#endif /* !_SHADER_INL_ */
