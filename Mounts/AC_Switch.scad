$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 51;
width = 26;
height = 2;

mounting_hole_length = 6;
mounting_hole_width = 6;
mounting_hole_height = 4;

screw_hole_x = length/4;
screw_hole_y = width/2;

difference()
{
    difference()
    {
        generic_board_m3(length, width, height, mounting_hole_length, mounting_hole_width, mounting_hole_height);
        union()
        {
            translate([screw_hole_x, screw_hole_y, 0])
            wood_hole();
            
            translate([length - screw_hole_x, screw_hole_y, 0])
            wood_hole();
        }
    }
}

translate([(3-3/4), width + 5, 0])
for(i = [0 : 1])
{
    for(j = [0 : 1])
    {
        translate([i*5, j*5, 0]) washer(3);
        translate([i*5, j*5, 0]) washer(3);
    }
}