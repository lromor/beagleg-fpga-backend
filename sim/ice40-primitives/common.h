#ifndef _COMMON_H
#define _COMMON_H

#include <memory>

template<typename Inputs, typename Outputs>
class DpiController {
public:
  class EventListener {
  public:
    virtual void Trigger(const Inputs *inputs, Outputs *outputs) = 0;
  };

  constexpr static void SetBackend(EventListener *backend) {
    backend_ = backend; }
 
  constexpr static void DpiEntrypoint(const Inputs *inputs, Outputs *outputs) {
    if (backend_ != NULL)
      backend_->Trigger(inputs, outputs);
  }

private:
  static inline EventListener *backend_ = NULL;
  DpiController();
};

#endif // _COMMON_H
