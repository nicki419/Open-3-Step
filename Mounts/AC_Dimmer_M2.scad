$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 53;
width = 27;
height = 2;

mounting_hole_length = 6;
mounting_hole_width = 6;

screw_hole_x = 20;
screw_hole_y = 5;

difference()
{
    generic_board(length, width, height, mounting_hole_length, mounting_hole_width);
    union()
    {
        translate([screw_hole_x, screw_hole_y, 0])
        wood_hole();
        
        translate([length - screw_hole_x, width - screw_hole_y, 0])
        wood_hole();
    }
}

translate([(3-3/4), width + 5, 0])
for(i = [0 : 1])
{
    for(j = [0 : 1])
    {
        translate([i*5, j*5, 0]) washer();
        translate([i*5, j*5, 0]) washer();
    }
}

module washer()
{
    difference()
    {
        cylinder(r=3-3/4, h=1.5);
        cylinder(r=1, h=1.5);
    }
}
