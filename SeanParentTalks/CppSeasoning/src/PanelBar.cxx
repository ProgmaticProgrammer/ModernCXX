#include <memory>
#include <vector>
using std::unique_ptr;
using std::vector;
class Panel {
 public:
  size_t cur_panel_center() const;
};

#define CHECK(x)

template <typename T>
struct ref_ptr {
  T* get() const { return nullptr; }
};

class PanelBar {
 private:
  vector<ref_ptr<Panel>> expanded_panels_;

 public:
  PanelBar(/* args */);
  ~PanelBar();
  void RepositionExpandedPanels(Panel* fixed_panel);
};

PanelBar::PanelBar(/* args */) {}

PanelBar::~PanelBar() {}
size_t center_x;
size_t fixed_index;

void PanelBar::RepositionExpandedPanels(Panel* fixed_panel) {
  CHECK(fixed_panel);

  // Next, check if the panel has moved to the other side of another panel.
  for (size_t i = 0; i < expanded_panels_.size(); ++i) {
    Panel* panel = expanded_panels_[i].get();
    if (center_x <= panel->cur_panel_center() ||
        i == expanded_panels_.size() - 1) {
      break;
    }
  }
  // Fix me, panel is from above loop
  if (panel != fixed_panel) {
    // If it has, then we reorder the panels.
    ref_ptr<Panel> ref = expanded_panels_[fixed_index];
    expanded_panels_.erase(expanded_panels_.begin() + fixed_index);
    expanded_panels_.insert(expanded_panels_.begin() + i, ref);
  }
}