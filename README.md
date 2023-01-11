# 3DViewer v1.0
A project for School 21.

### Building
Requires OpenGL 3.3+ and [GLFW](https://github.com/glfw/glfw) to run.
On Linux, install GLFW and `gtk-devel` (for "open file" dialog window). On Mac, just install GLFW.

__Debian__ (didn't test myself, but should work):
```shell
sudo apt install libglfw3-dev libgtk-3-dev
```
__Fedora__:
```shell
sudo dnf install glfw-devel gtk3-devel
```

Then run `make`.

### Roadmap
- [X] Translate/Rotate/Scale as separate operations on the model
	- Set correct translation/rotation/scale in `t_object`, call `update_view_mat()`, set new MVP matrix uniform
- [ ] Use mouse to rotate the model (optional)
- [X] UI
	- Works. Sort of.
- [ ] `.obj` file parsing, loading, correct error handling
- [ ] Unit tests for model loading and affine transformations
- [ ] Makefile rules: `install`, `uninstall`, `dvi`, `dist`, `tests`, `gcov_report`
