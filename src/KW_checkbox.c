#include "KW_checkbox.h"
#include "KW_label.h"
#include "KW_toggle.h"

typedef struct KW_Checkbox {
  KW_Widget * label;
  KW_Widget * toggle;
} KW_Checkbox;

static void PaintCheckbox(KW_Widget * widget, const KW_Rect * absolute, void * data);
static void DestroyCheckbox(KW_Widget * widget);
static void GeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom);
static void MouseUp(KW_Widget * widget, int b);

KW_Widget * KW_CreateCheckbox(KW_GUI * gui, KW_Widget * parent, KW_Widget * label, const KW_Rect * geometry) {
  KW_Checkbox * cb = calloc(1, sizeof(KW_Checkbox));
  KW_Widget * root = KW_CreateWidget(gui, parent, geometry, PaintCheckbox, DestroyCheckbox, cb);
  cb->label = label;
  cb->toggle = KW_CreateToggle(gui, root, geometry);
  KW_ReparentWidget(label, root);
  GeometryChanged(root, geometry, NULL);
  KW_AddWidgetGeometryChangeHandler(root, GeometryChanged);
  KW_AddWidgetMouseUpHandler(cb->label, MouseUp);
  return root;
}

void GeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom) {
  KW_Checkbox * cb = KW_GetWidgetData(widget, PaintCheckbox);
  KW_Rect labelgeom, togglegeom;
  KW_Rect * geoms[] = { NULL, NULL };
  unsigned weights[] = { 0, 0 };
  (void) oldgeom;
  geoms[0] = &togglegeom; geoms[1] = &labelgeom;
  weights[0] = (unsigned) ((30.0f/newgeom->w)*100.0f);
  weights[1] = (unsigned) (((newgeom->w - 30.0f)/newgeom->w)*100.0f);
  KW_GetWidgetGeometry(cb->label, &labelgeom);
  if (labelgeom.h == 0) labelgeom.h = newgeom->h;
  KW_GetWidgetGeometry(cb->toggle, &togglegeom);
  togglegeom.h = 30;
  KW_RectFillParentHorizontally(newgeom, geoms, weights, 2, 0, KW_RECT_ALIGN_MIDDLE);
  KW_SetWidgetGeometry(cb->label, &labelgeom);
  KW_SetWidgetGeometry(cb->toggle, &togglegeom);
}

void PaintCheckbox(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  (void) widget;
  (void) absolute;
  (void) data;
}

void DestroyCheckbox(KW_Widget * widget) {
  free(KW_GetWidgetData(widget, PaintCheckbox));
}

static void MouseUp(KW_Widget * widget, int b) {
  KW_Checkbox * cb = KW_GetWidgetData(KW_GetWidgetParent(widget), PaintCheckbox);
  (void) b;
  KW_SetToggleChecked(cb->toggle, (KW_bool) !KW_IsToggleChecked(cb->toggle));
}