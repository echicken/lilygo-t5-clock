batt_w = 114;  // Battery width
batt_h = 61;   // Battery height
batt_t = 11;   // Battery thickness
batt_lip = 10; // Battery channel overlaps this much of the battery on each side

epd_w = 117; // Width of EPD module (board, display, tape)
epd_h = 64;  // Height of EPD module (board, display, excludes USB-C projection)
epd_tl = 4;  // Thickness of EPD module on left-hand side
epd_tr = 6;  // Thickness of EPD module on right-hand side
epd_tf = 10; // Full thickness / thickest measurement of EPD module including mounted components (exaggerated)
epd_lip = 4; // EPD module channel overlaps this much of the EPD module on each side

// USB-C measurements have been exaggerated to make it easier to plug in
usb_ol = 4;          // USB-C connector offset from left of board
usb_w = 12;          // USB-C connector width
usb_t = 6;           // USB-C connector thickness
usb_of = epd_tl - 1; // USB-C connector offset from front

disp_or = 9; // Display-area offset from right side of EPD module
disp_ol = 3; // Display-area offset from left side of EPD module
disp_ob = 3; // Display-area offset from bottom of EPD module
disp_ot = 3; // Display-area offset from top of EPD module

vwt = 1.67;  // Vertical wall thickness
hwt = 2;     // Horizontal wall thickness
cr = 2;      // Corner radius
pd = 3;      // Post diameter (for lid press-fit)
pr = pd / 2; // Post radius
phz = 5;     // Post hole height
pz = 4;      // Post height
wd = 2;      // Hanging wire diameter

bl = 25; // Button offset from left of EPD module
bw = 8;  // Button width (exaggerated for easier access)
by = 5;  // Button hole y axis (exaggerated for easier access)

tol = .127; // Tolerance for post hole and lid bezel fit

ch = hwt + epd_h; // Case height
cw = vwt + epd_w + vwt; // Case width
ct = vwt + epd_tf + vwt + batt_t + vwt; // Case thickness

$fn = 180;

module block() {
    hull() {
        translate([cr, cr, 0]) {
            cylinder(h = ch, r = cr);
        }
        translate([cw - cr, cr, 0]) {
            cylinder(h = ch, r = cr);
        }
        translate([cr, ct - cr, 0]) {
            cylinder(h = ch, r = cr);
        }
        translate([cw - cr, ct - cr, 0]) {
            cylinder(h = ch, r = cr);
        }
    }
}

module wire_holes() {
    translate([0, ct, ch * .66]) {
        rotate([90, 0, 0]) {
            cylinder(d = wd, h = vwt * 2);
        }
        translate([0, 0, -(wd * 2)]) {
            rotate([90, 0, 0]) {
                cylinder(d = wd, h = vwt * 2);
            }
        }
    }
}

module cutouts() {
    // Display area
    translate([vwt + disp_ol, 0, hwt + disp_ob]) {
        cube([cw - (vwt * 2) - disp_ol - disp_or, vwt, ch - hwt - disp_ob]);
    }
    // EPD module left cutout
    translate([vwt, vwt, hwt]) {
        cube([epd_w / 2, epd_tl, ch]);
    }
    // EPD module right cutout
    translate([vwt + (epd_w / 2), vwt, hwt]) {
        cube([epd_w / 2, epd_tr, ch]);
    }
    // EPD lip cutout
    translate([vwt + epd_lip, vwt + epd_tl, hwt]) {
        cube([epd_w - (epd_lip * 2), epd_tr, ch]);
    }
    // Battery cutout
    translate([vwt, vwt + epd_tf + vwt, hwt]) {
        cube([batt_w, batt_t, ch]);
    }
    // Battery lip cutout
    translate([vwt + batt_lip, vwt + epd_tf, hwt]) {
        cube([batt_w - (batt_lip * 2), vwt, ch]);
    }
    // USB-C cutout
    translate([vwt + usb_ol, vwt + usb_of, 0]) {
        cube([usb_w, usb_t, hwt]);
    }
    // Lid post holes
    translate([vwt + pr, vwt + epd_tr + vwt + pr, ch - phz]) {
        cylinder(d = pd, h = phz);
    }
    translate([cw - vwt - pr, vwt + epd_tr + vwt + pr, ch - phz]) {
        cylinder(d = pd, h = phz);
    }
    // Wire hanger holes
    translate([cw * .25, 0, 0]) wire_holes();
    translate([cw * .75, 0, 0]) wire_holes();
}

module case() {
    difference() {
        block();
        cutouts();
    }
}

module lid() {
    difference() {
        union() {
            hull() {
                translate([cr, cr, 0]) {
                    cylinder(h = hwt, r = cr);
                }
                translate([cw - cr, cr, 0]) {
                    cylinder(h = hwt, r = cr);
                }
                translate([cr, ct - cr, 0]) {
                    cylinder(h = hwt, r = cr);
                }
                translate([cw - cr, ct - cr, 0]) {
                    cylinder(h = hwt, r = cr);
                }
            }
            // Bezel strip
            translate([vwt + disp_ol + (tol / 2), 0, -disp_ot]) {
                cube([cw - (vwt * 2) - disp_ol - disp_or - tol, vwt, disp_ot]);
            }
            // Lid posts
            translate([vwt + pr, vwt + epd_tr + vwt + pr, -pz]) {
                cylinder(d = pd - tol, h = pz);
            }
            translate([cw - vwt - pr, vwt + epd_tr + vwt + pr, -pz]) {
                cylinder(d = pd - tol, h = pz);
            }
        }
        // Button pokin' hole
        translate([vwt + bl, vwt + epd_tl - 1, 0]) {
            cube([bw, by, hwt]);
        }
    }
}

case();
translate([0, 0, ch]) lid();