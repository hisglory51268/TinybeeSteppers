= Stepper Library for use with mkstinybee =

library for managing stepper motors connected to a MKS tinybee
because all I/Os 128-151 are sent through the shift registers it was easier create a library handle the steppers and the I/Os 144-151

the I/Os are outputs only. Designed for the MKS tinybee however the library could be modified to work with other boards using shift registers as well
also the motors have been renamed to 1-5

== License ==

/*  tinybeeSteppers.h -
 copyright (c) Jeffrey Rydell All right reserved.
*/

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
