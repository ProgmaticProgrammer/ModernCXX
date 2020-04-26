#include <algorithm>
#include <memory>
#include <vector>
using std::find_if;
using std::rotate;
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
  T* operator->() const { return nullptr; }
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
  Panel* panel = nullptr;
  // Next, check if the panel has moved to the other side of another panel.
  auto p =
      find_if(std::begin(expanded_panels_), std::end(expanded_panels_),
              [&](const auto& e) { return center_x <= e->cur_panel_center(); });

  // If it has, then we reorder the panels.
  auto f = expanded_panels_.begin() + fixed_index;
  rotate(p, f, f + 1);
}