#define PWM_MAX 100
#define INTEGERATION_TIME_MS    10
#define GYRO_OFFSET_250     (0.3)
#define GYRO_OFFSET_500     (-0.16)
#define GYRO_OFFSET_2000    (-0.52)

//Functions Prototypes
void Init();
void MOTORS (uint8_t direction, uint8_t duty);