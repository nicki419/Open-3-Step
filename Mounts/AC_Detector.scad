$fn = $preview ? 0 : 100;

include <_modules.scad>;
include <_generic_board.scad>;

length = 72;
width = 17;
height = 2;

mounting_hole_length = 6;
mounting_hole_width = 6;

screw_hole_x = 20;
screw_hole_y = 5;

difference()
{
    cube([length, width, height]);
    union()
    {
        translate([4,8.5,0])
        wood_hole();
        
        translate([length - 4,8.5,0])
        wood_hole();
    }
}

// 3, 18
translate([18, width - 3, 2])
mounting_hole(mounting_hole_length, mounting_hole_width);

// 7, 18
translate([length - 18, width - 7, 2])
mounting_hole(mounting_hole_length, mounting_hole_width);