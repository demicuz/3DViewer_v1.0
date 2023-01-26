# 3DViewer v1.0
A project for School 21.

### Building
Requires OpenGL 3.3+ and [GLFW](https://github.com/glfw/glfw) to run.
On Linux, install GLFW and `gtk-devel` (for "open file" dialog window). On Mac, just install GLFW.

__Debian__ (didn't test myself, but should work):
```shell
sudo apt install libglfw3-dev libgtk-3-dev libcheck-dev
```
__Fedora__:
```shell
sudo dnf install glfw-devel gtk3-devel check-devel
```

Then run `make`.

`models/` has some sample `.obj` files. You can find more at [free3d.com](https://free3d.com/3d-models/simple).

### Roadmap
- [X] Translate/Rotate/Scale as separate operations on the model
	- Set correct translation/rotation/scale in `t_object`, call `update_view_mat()`, set new MVP matrix uniform
- [ ] Use mouse to rotate the model (optional)
- [X] UI
	- Works. Sort of.
- [X] `.obj` file parsing, loading, correct error handling
	- [ ] Support for negative vertex indices (will require a second pass during parsing)
- [X] Unit tests for model loading and affine transformations
- [ ] `make install`
- [ ] `make uninstall`
- [ ] `make dvi`
- [ ] `make dist`
- [X] `make tests`
- [ ] `make gcov_report`
