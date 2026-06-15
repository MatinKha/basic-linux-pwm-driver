#include <linux/init.h>
#include <linux/module.h>
#include <linux/pwm.h>

static struct pwm_device *pwm;

static int __init pwm_init(void) {
  struct pwm_state state;
  int ret;

  // for testing
  pwm = pwm_get(NULL, "virtual-pwm");
  if (IS_ERR(pwm))
    return PTR_ERR(pwm);

  pwm_init_state(pwm, &state);

  state.period = 1000000;  
  state.duty_cycle = 500000;
  state.enabled = true;

  ret = pwm_apply_might_sleep(pwm, &state);
  if (ret)
    goto err;

  pr_info("PWM configured\n");

  pwm_get_state(pwm, &state);

  pr_info("enabled:%d period:%llu duty:%llu\n", state.enabled, state.period,
          state.duty_cycle);

  return 0;

err:
  pwm_put(pwm);
  return ret;
}

static void __exit pwm_exit(void) {
  struct pwm_state state;

  if (IS_ERR_OR_NULL(pwm))
    return;

  pwm_get_state(pwm, &state);
  state.enabled = false;

  pwm_apply_might_sleep(pwm, &state);

  pwm_put(pwm);

  pr_info("PWM released\n");
}

module_init(pwm_init);
module_exit(pwm_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("virtual PWM consumer");
