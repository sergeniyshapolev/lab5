#include <linux/module.h>
#include <linux/hrtimer.h>

int interval = 0;
struct hrtimer htimer;
ktime_t kt_periode;

enum hrtimer_restart timer_function(struct hrtimer * unused) {
	if (interval != 0) {
		printk(KERN_ALERT "Hello, world!\n" );
		kt_periode = ktime_set(interval, 0);
	}
	hrtimer_forward_now(&htimer, kt_periode);
	return HRTIMER_RESTART;
}

void timer_init(void) {
	kt_periode = ktime_set(1, 0);
	hrtimer_init(&htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	htimer.function = timer_function;
	hrtimer_start(&htimer, kt_periode, HRTIMER_MODE_REL);
}

ssize_t write(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count) {
	sscanf(buf, "%d", &interval);
	return count;
}

struct kobj_attribute attribute = __ATTR(interval, 0666, NULL, write);
struct attribute *attrs[] = { &attribute.attr, NULL };
struct attribute_group attr_group = { .attrs = attrs, };
struct kobject *timer_kobj;

int __init init(void)
{
	int retval;

	timer_init();

	timer_kobj = kobject_create_and_add("kobject_timer", kernel_kobj);
	if (!timer_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(timer_kobj, &attr_group);
	if (retval)
		kobject_put(timer_kobj);

	return retval;
}

void __exit exit(void)
{
	kobject_put(timer_kobj);
	hrtimer_cancel(&htimer);
}

module_init(init);
module_exit(exit);
MODULE_LICENSE("GPL");
