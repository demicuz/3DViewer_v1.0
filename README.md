# 3DViewer v1.0

A project for School 21.

### Roadmap
- [X] Translate/Rotate/Scale as separate operations on the model
	- Set correct translation/rotation/scale in `t_object`, call `update_view_mat()`, set new MVP matrix uniform
- [ ] Use mouse to rotate the model (optional)
- [ ] UI
- [ ] `.obj` file parsing, loading, correct error handling
- [ ] Unit tests for model loading and affine transformations
- [ ] Makefile rules: `install`, `uninstall`, `dvi`, `dist`, `tests`, `gcov_report`
