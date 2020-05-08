#ifndef _COMMON_H
#define _COMMON_H

#include <memory>

template<typename Inputs, typename Outputs>
class DpiController {
public:
  class EventListener {
    friend class DpiController;
  private:
    virtual void trigger(const Inputs *inputs, Outputs *outputs) = 0;
  };

  static void SetBackend(EventListener *backend) {
    backend_ = backend; }
 
  static void dpi_entrypoint(const Inputs *inputs, Outputs *outputs) {
    if (backend_ != NULL)
      backend_->trigger(inputs, outputs);
  }

private:
  static inline EventListener *backend_ = NULL;
  DpiController();
};

#endif // _COMMON_H
