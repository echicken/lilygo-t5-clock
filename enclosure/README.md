# Enclosure

- [./assembled.stl](assembled.stl) is here for illustrative purposes, so you can see the assembled case. Don't print it.
- [./case.stl](case.stl) is the main body of the case, to be printed as shown with the open top facing up. No supports needed. The battery and EPD module just slide into their channels.
- [./lid.stl](lid.stl) has two posts that press fit into sockets on the case body. On my printed part, this holds the lid flat and secure and no glue or fasteners are needed. Print this with the posts and bezel strip facing up (obviously).
- [./case.scad](case.scad) is the OpenSCAD source for this design so you can tinker with parameters if needed.

I printed my case in eSun transparent Blue PETG on a Prusa i3 mk3. Your results may differ depending on your printer and material. In particular you may want to tweak the `tol` and `vwt` parameters in [./case.scad](case.scad) for best results. The `tol` parameter will affect the fit of the lid posts and lid bezel strip.