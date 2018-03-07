/*
MD_Cubo - Arduino LED Cube Library
  
See main header file for comments

This file contains class related methods.
  
Copyright (C) 2015 Marco Colli. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <Arduino.h>
#include "MD_Cubo.h"
#include "MD_Cubo_lib.h"

/**
 * \file
 * \brief Implements class definition and general methods
 */

void MD_Cubo::clear(uint32_t p)
{
  for (uint8_t i=0; i<_sizeXaxis; i++)
    for (uint8_t j=0; j<_sizeYaxis; j++)
      for (uint8_t k=0; k<_sizeZaxis; k++)
        setVoxel(p, i, j, k);
}

void MD_Cubo::fillPlane(uint32_t p, plane_t plane, uint8_t coord)
// Fill the specified plane at the coordinate value
{
  PRINT("\nfillPlane ", plane);
  PRINT(" coord ", coord);
  PRINT(" value ", p);
  switch (plane)
  {
    case XYPLANE: 
      for (uint8_t i = 0; i < _sizeXaxis; i++)
        for (uint8_t j = 0; j < _sizeYaxis; j++)
          setVoxel(p, i, j, coord); 
      break;
    case XZPLANE:
      for (uint8_t i = 0; i < _sizeXaxis; i++)
        for (uint8_t j = 0; j < _sizeZaxis; j++)
          setVoxel(p, i, coord, j); 
      break;
    case YZPLANE:
      for (uint8_t i = 0; i < _sizeYaxis; i++)
        for (uint8_t j = 0; j < _sizeZaxis; j++)
          setVoxel(p, coord, i, j);
      break;
  }
}

void MD_Cubo::copyPlane(plane_t plane, uint8_t cordFrom, uint8_t cordTo)
// copy the plane from level cordFrom to cordTo
{
  PRINT("\ncopyPlane ", plane);
  PRINT(" from ", cordFrom);
  PRINT(" to ", cordTo);

  switch (plane)
  {
  case XYPLANE:
    for (uint8_t i = 0; i < _sizeXaxis; i++)
      for (uint8_t j = 0; j < _sizeYaxis; j++)
        setVoxel(getVoxel(i, j, cordFrom), i, j, cordTo);
    break;
  case XZPLANE:
    for (uint8_t i = 0; i < _sizeXaxis; i++)
      for (uint8_t j = 0; j < _sizeZaxis; j++)
        setVoxel(getVoxel(i, cordFrom, j), i, cordTo, j);
    break;
  case YZPLANE:
    for (uint8_t i = 0; i < _sizeYaxis; i++)
      for (uint8_t j = 0; j < _sizeZaxis; j++)
        setVoxel(getVoxel(cordFrom, i, j), cordTo, i, j);
    break;
  }
}

void MD_Cubo::drawLine(uint32_t p, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2)
// Bresenham 3D line algorithm
{
  int8_t  i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
  int8_t  point[3];

  point[0] = x1;   point[1] = y1;   point[2] = z1;
  dx = x2 - x1;    dy = y2 - y1;    dz = z2 - z1;
  x_inc = (dx < 0) ? -1 : 1;        l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;        m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;        n = abs(dz);
  dx2 = l << 1;    dy2 = m << 1;    dz2 = n << 1;

  if ((l >= m) && (l >= n))
  {
    err_1 = dy2 - l;
    err_2 = dz2 - l;
    for (i = 0; i < l; i++)
    {
      setVoxel(p, point[0], point[1], point[2]);
      if (err_1 > 0)
      {
        point[1] += y_inc;
        err_1 -= dx2;
      }
      if (err_2 > 0)
      {
        point[2] += z_inc;
        err_2 -= dx2;
      }
      err_1 += dy2;
      err_2 += dz2;
      point[0] += x_inc;
    }
  }
  else if ((m >= l) && (m >= n))
  {
    err_1 = dx2 - m;
    err_2 = dz2 - m;
    for (i = 0; i < m; i++)
    {
      setVoxel(p, point[0], point[1], point[2]);
      if (err_1 > 0)
      {
        point[0] += x_inc;
        err_1 -= dy2;
      }
      if (err_2 > 0)
      {
        point[2] += z_inc;
        err_2 -= dy2;
      }
      err_1 += dx2;
      err_2 += dz2;
      point[1] += y_inc;
    }
  }
  else
  {
    err_1 = dy2 - n;
    err_2 = dx2 - n;
    for (i = 0; i < n; i++)
    {
      setVoxel(p, point[0], point[1], point[2]);
      if (err_1 > 0)
      {
        point[1] += y_inc;
        err_1 -= dz2;
      }
      if (err_2 > 0)
      {
        point[0] += x_inc;
        err_2 -= dz2;
      }
      err_1 += dy2;
      err_2 += dx2;
      point[2] += z_inc;
    }
  }
  setVoxel(p, point[0], point[1], point[2]);
}

void MD_Cubo::drawRPrism(uint32_t p, uint8_t x, uint8_t y, uint8_t z, int8_t dx, int8_t dy, int8_t dz)
{
  // top rectangle
  drawLine(p, x, y, z, x, y+dy, z);
  drawLine(p, x, y+dy, z, x+dx, y+dy, z);
  drawLine(p, x+dx, y+dy, z, x+dx, y, z);
  drawLine(p, x+dx, y, z, x, y, z);

  // bottom rectangle
  drawLine(p, x, y, z+dz, x, y+dy, z+dz);
  drawLine(p, x, y+dy, z+dz, x+dx, y+dy, z+dz);
  drawLine(p, x+dx, y+dy, z+dz, x+dx, y, z+dz);
  drawLine(p, x+dx, y, z+dz, x, y, z+dz);

  // joining verticals
  drawLine(p, x, y, z, x, y, z+dz);
  drawLine(p, x, y+dy, z, x, y+dy, z+dz);
  drawLine(p, x+dx, y+dy, z, x+dx, y+dy, z+dz);
  drawLine(p, x+dx, y, z, x+dx, y, z+dz);
}
