
/*
 * 
 * Simlib Simulation Library
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/**********************************************************************/

#ifndef _TRACE_H_
#define _TRACE_H_

/**********************************************************************/

#define DO(x) x
#define IGNORE(x)

/* Uncomment the next statement to activate trace. */
/* #define TRACE_ON */

#ifdef TRACE_ON
#define TRACE DO
#else
#define TRACE IGNORE
#endif

#define TRACEF(a) { printf("%s @ line %u\n", __FILE__, __LINE__); \
    a;								  \
    fflush(stdout); }

/**********************************************************************/

#endif /* trace.h */


