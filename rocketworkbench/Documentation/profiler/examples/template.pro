
global = (length_units  = "meter"
	 ,mass_units    = "kg"
         )

rocket = (name =       "Meteor"
	 ,cg   =       1.0
         ,body =       (type       = "conic_nose" # Ogive, Elliptic
                                                  # Conic, Parabolic
                       ,length     = 0.3     # meter
                       ,diameter   = 0.1     # meter
		                             # one of those
                       ,mass       = 0.1     # kg
                       )
         ,body       = (type       = "tube"
                       ,length     = 1.3
                       ,mass       = 0.1
                       )
         ,fin        = (num        = 4
                       ,pos        = 1.4 # distance from nose tip to leading 
                                         # edge at the root
                       ,root_chord = 0.2
                       ,tip_chord  = 0.1
                                         # distance between root and tip 
                                         # leading edge measured parallel to 
                                         # the rocket
                       ,root_tip_d = 0.2 
                       ,semispan   = 0.2
		       ,body_radius = 0.05
		       ,mass        = 0.05 # of one fin
                       )
         ,motor      = (zpos       = 2.5 # distance from nose tip
         	       ,rpos       = 0.0 # axial distance
                       ,diameter   = 0.04
                       ,length     = 0.35
                       ,empty_mass = 1.0
                       ,full_mass  = 10.0
                       )
         ,tank       = (zpos       = 1.
                       ,rpos       = 0.0
                       ,diameter   = 0.05
                       ,length     = 1.
                       ,empty_mass = 1.0
                       ,full_mass  = 10.0
                       )
         ,other     = (shape       = "thick_cylinder"
                      ,zpos        = 0.2
                      ,rpos        = 0.0
                      ,Ix          = 1.
                      ,Iy          = 1.
                      ,mass        = 5.
                      )
         )


#rocket = (name =       "Meteor",
#	  body =       (type       = "ogive_nose"  #Ogive, Elliptic
#                                                   # Conic, Parabolic
#                       ,length     = 0.3
#                       ,diameter   = 0.06
#                       ,density   = 0.3
#                       )
#         ,body =       (type       = "tube"
#                       ,length     = 1.5
#                       ,density    = 0.3
#                       )
#         ,body =       (type       = "transition"
#                       ,length     = 0.2
#                       ,density    = 0.3
#
#                       )
#         ,body =       (type       = "tube"
#                       ,length     = 1.
#        	       ,diameter   = 0.1
#                       ,density    = 0.3
#                       )
#         ,fin        = (num        = 4
#                       ,pos        = 2.5 # position of the beginning of the root
#                       ,root_chord = 0.2
#                       ,tip_chord  = 0.05
                                         # distance between root and tip 
                                         # leading edge measured parallel to 
                                         # the rocket
#                       ,root_tip_d = 0.03 
#                       ,semispan   = 0.02
#                       )
#         ,motor      = (zpos       = 2.5 # distance from nose tip
#         	       ,rpos       = 0.0 # axial distance
#                       ,diameter   = 0.04
#                       ,length     = 0.35
#                       ,empty_mass = 1.0
#                       ,full_mass  = 10.0
#                       )
#         ,tank       = (zpos       = 1.
#                       ,rpos       = 0.0
#                       ,diameter   = 0.05
#                       ,length     = 1.
#                       ,empty_mass = 1.0
#                       ,full_mass  = 10.0
#                       )
#         ,other     = (shape       = "thick_cylinder"
#                      ,zpos        = 0.2
#                      ,rpos        = 0.0
#                      ,Ix          = 1.
#                      ,Iy          = 1.
#                      ,mass        = 5.
#                      )
#         )

