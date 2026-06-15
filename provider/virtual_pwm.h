
#ifndef VIRTUAL_PWM_H
#define VIRTUAL_PWM_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pwm.h>
#define PWM_PROVIDER "virtual_provider"

struct virtual_pwm {
  struct pwm_chip chip;
  struct pwm_state state;
  struct mutex lock;
};

static inline struct virtual_pwm *to_virtual_pwm(struct pwm_chip *chip) {
  return container_of(chip, struct virtual_pwm, chip);
}

static int virtual_pwm_apply(struct pwm_chip *chip, struct pwm_device *device,
                             const struct pwm_state *state) {
  struct virtual_pwm *vpwm = to_virtual_pwm(chip);

  mutex_lock(&vpwm->lock);

  // TODO GPIO or Register calls here
  vpwm->state = *state;

  mutex_unlock(&vpwm->lock);

  return 0;
}

static int virtual_pwm_get_state(struct pwm_chip *chip,
                                 struct pwm_device *device,
                                 struct pwm_state *state) {
  struct virtual_pwm *vpwm = to_virtual_pwm(chip);

  mutex_lock(&vpwm->lock);

  *state = vpwm->state;

  mutex_unlock(&vpwm->lock);

  return 0;
}

// these functions are provided to the consumer
// consumer is able to set the state with apply and get the state with get_state
// for example you can read values from a gpio port using kernel's gpio
static const struct pwm_ops pwm_ops = {
    .apply = virtual_pwm_apply,
    .get_state = virtual_pwm_get_state,
};
#endif
