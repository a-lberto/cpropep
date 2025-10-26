
  altitude  = 0; # altitude in meter
  latitude  = 15; # latitude in decimal degree
  longitude = 0;    # longitude in decimal degree

  velocity  = 1000; # magnitude of the initial velocity in meter/s
  angle     = 0;   # angle with the horizontal in degree
  direction = 90;   # direction (0=east 90=north 180=west 270=south)

  R = 6.37e6; # earth radius in meter

  r = R + altitude;
  r_dot = velocity*sin(angle*pi/180);
  u_dot = velocity*cos(angle*pi/180)*cos(direction*pi/180)/(r*cos(latitude*pi/180));
  l_dot = velocity*cos(angle*pi/180)*sin(direction*pi/180)/r;

  init = [ r; latitude*pi/180; longitude*pi/180; r_dot; l_dot; u_dot; ];

  t = 0:0.1:200;

  y = lsode('flight_vacuum', init, t);

  hold off
  plot(y(:,1).*cos(y(:,2)), y(:,1).*sin(y(:,2)));
  hold on;
  plot(R*cos(y(:,2)), R*sin(y(:,2)));

  #plot(y(:,3)*180/pi, y(:,2)*180/pi);



