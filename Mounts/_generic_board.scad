module generic_board_m3(
length,
width,
height,
mounting_hole_length,
mounting_hole_width,
mounting_hole_height = 3
)
{
    cube([length, width, height]);

    translate([mounting_hole_length / 2, mounting_hole_width / 2, height])
    mounting_hole_m3(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([mounting_hole_length / 2, width - (mounting_hole_width / 2), height])
    mounting_hole_m3(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([length - (mounting_hole_length / 2), mounting_hole_width / 2, height])
    mounting_hole_m3(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([length - (mounting_hole_length / 2), width - (mounting_hole_width / 2), height])
    mounting_hole_m3(mounting_hole_length, mounting_hole_width, mounting_hole_height);
}

module generic_board(
length,
width,
height,
mounting_hole_length,
mounting_hole_width,
mounting_hole_height = 3
)
{
    cube([length, width, height]);

    translate([mounting_hole_length / 2, mounting_hole_width / 2, height])
    mounting_hole(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([mounting_hole_length / 2, width - (mounting_hole_width / 2), height])
    mounting_hole(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([length - (mounting_hole_length / 2), mounting_hole_width / 2, height])
    mounting_hole(mounting_hole_length, mounting_hole_width, mounting_hole_height);

    translate([length - (mounting_hole_length / 2), width - (mounting_hole_width / 2), height])
    mounting_hole(mounting_hole_length, mounting_hole_width, mounting_hole_height);
}