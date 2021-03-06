#include <platform.h>
#include <irqs.h>


#if defined(CONFIG_PC9223)
#define SQ_PWM_NUM			2
#else
#define SQ_PWM_NUM			1
#endif

#if defined(CONFIG_PC9223)
#define SUPT_CH			5
#else
#define SUPT_CH			8
#endif


// use pwm number to test adc
#if defined(CONFIG_PC9223)
#define USE_PWM_NUM			1
#else
#define USE_PWM_NUM			0
#endif

#define DEFAULT_PRE_SCL		0x0		// default Prescale Factor
#define DEFAULT_HRC			0x64	// default HRC
#define DEFAULT_LRC			0x64	// default LRC


// use pwm number to test capture mode
#define USE_PWM_NUM_0		0
#define USE_PWM_NUM_1		1


#ifdef SQ_INTC_PWM0
#define SQ_PWMT_0_ENABLE		1
#else
#define SQ_PWMT_0_ENABLE		0
#endif

#ifdef SQ_INTC_PWM1
#define SQ_PWMT_1_ENABLE		1
#else
#define SQ_PWMT_1_ENABLE		0
#endif

#ifdef SQ_INTC_PWM2
#define SQ_PWMT_2_ENABLE		1
#else
#define SQ_PWMT_2_ENABLE		0
#endif

#ifdef SQ_INTC_PWM3
#define SQ_PWMT_3_ENABLE		1
#else
#define SQ_PWMT_3_ENABLE		0
#endif

#define SQ_PWMT_CURRENT_SENSE_TEST 0
#define SQ_PWMT_CAPTURE_LOOP 1




