##
##  Copyright(c) Laika, developed under contract by mitchpraterLLC.
##  All rights reserved unless otherwise granted. This program or documentation
##  contains proprietary, confidential information and trade secrets of Laika
##  and/or mitchpraterLLC. Use of copyright notice does not imply publication.
##  
##  By possessing this software, you agree to the following terms and conditions.
##  You may not resell this software, but you may freely distribute it to others.
##  You may use it for whatever purposes you wish. There are no warranties of any
##  kind. You may make modifications, however any derivation may only be distributed
##  to others under the same freedoms granted to you herein, it must retain this
##  notice and agreement, and the modifications noted. We also ask that you make
##  such modifications available to Laika, mitchpraterLLC, or to the computer
##  graphics community as a whole through a public forum.
##

The entire contents of the Laika Plausible Shading Toolkit distribution are subject
to the above end user license agreement.

Installation Instructions:

Extract the contents to a suitable directory, such as /opt/laika/shading.

Three environment variables must be set in order to make use of the Laika Plausible
Shading Toolkit: RMS_SCRIPT_PATHS, RMSTREE, and RMANTREE.

RMS_SCRIPT_PATHS must be set to the directory containing the extracted distribution contents.
For example, in bash:

export RMS_SCRIPT_PATHS="/opt/laika/shading"

in tcsh:

setenv RMS_SCRIPT_PATHS /opt/laika/shading

RMSTREE must be set to the location of your Pixar RMS installation.

RMANTREE must be set to the location of your Pixar PRMAN installation.
