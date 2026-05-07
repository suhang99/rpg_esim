#include <ze/common/logging.hpp>
#include <esim/trajectory/imu_factory.hpp>

DEFINE_string(simulation_imu_bias_mode, "random_walk",
              "IMU bias mode: zero, constant, or random_walk.");

DEFINE_double(simulation_imu_gyr_bias_noise_sigma, 0.0000266,
              "Gyroscope bias random-walk noise sigma.");
DEFINE_double(simulation_imu_acc_bias_noise_sigma, 0.000433,
              "Accelerometer bias random-walk noise sigma.");
DEFINE_double(simulation_imu_gyr_noise_sigma, 0.000186,
              "Gyroscope white noise sigma.");
DEFINE_double(simulation_imu_acc_noise_sigma, 0.00186,
              "Accelerometer white noise sigma.");

DEFINE_double(simulation_imu_gyr_bias_init_x, 0.0,
              "Initial constant gyroscope bias x.");
DEFINE_double(simulation_imu_gyr_bias_init_y, 0.0,
              "Initial constant gyroscope bias y.");
DEFINE_double(simulation_imu_gyr_bias_init_z, 0.0,
              "Initial constant gyroscope bias z.");
DEFINE_double(simulation_imu_acc_bias_init_x, 0.0,
              "Initial constant accelerometer bias x.");
DEFINE_double(simulation_imu_acc_bias_init_y, 0.0,
              "Initial constant accelerometer bias y.");
DEFINE_double(simulation_imu_acc_bias_init_z, 0.0,
              "Initial constant accelerometer bias z.");

DEFINE_uint32(simulation_imu_bias_samples, 100,
              "Number of samples used to seed the continuous IMU bias spline.");
DEFINE_uint32(simulation_imu_bandwidth_hz, 200,
              "IMU white noise bandwidth in Hz.");
DEFINE_double(simulation_imu_gravity_magnitude, 9.81,
              "Gravity magnitude used by the IMU simulator.");

namespace event_camera_simulator {

namespace {

ze::Vector3 loadGyroBiasInit()
{
  return ze::Vector3(
      FLAGS_simulation_imu_gyr_bias_init_x,
      FLAGS_simulation_imu_gyr_bias_init_y,
      FLAGS_simulation_imu_gyr_bias_init_z);
}

ze::Vector3 loadAccelBiasInit()
{
  return ze::Vector3(
      FLAGS_simulation_imu_acc_bias_init_x,
      FLAGS_simulation_imu_acc_bias_init_y,
      FLAGS_simulation_imu_acc_bias_init_z);
}

ze::ImuBiasSimulator::Ptr buildBiasSimulator(const ze::TrajectorySimulator::Ptr& trajectory)
{
  if (FLAGS_simulation_imu_bias_mode == "zero")
  {
    return std::make_shared<ze::ConstantBiasSimulator>(
        ze::Vector3::Zero(), ze::Vector3::Zero());
  }

  if (FLAGS_simulation_imu_bias_mode == "constant")
  {
    return std::make_shared<ze::ConstantBiasSimulator>(
        loadAccelBiasInit(), loadGyroBiasInit());
  }

  if (FLAGS_simulation_imu_bias_mode == "random_walk")
  {
    return std::make_shared<ze::ContinuousBiasSimulator>(
        ze::Vector3::Constant(FLAGS_simulation_imu_gyr_bias_noise_sigma),
        ze::Vector3::Constant(FLAGS_simulation_imu_acc_bias_noise_sigma),
        trajectory->start(),
        trajectory->end(),
        FLAGS_simulation_imu_bias_samples);
  }

  LOG(FATAL) << "Unsupported simulation_imu_bias_mode: "
             << FLAGS_simulation_imu_bias_mode
             << ". Expected one of: zero, constant, random_walk.";
  return nullptr;
}

} // namespace

ze::ImuSimulator::Ptr loadImuSimulatorFromGflags(const ze::TrajectorySimulator::Ptr& trajectory)
{
  ze::ImuSimulator::Ptr imu;

  ze::ImuBiasSimulator::Ptr bias;
  try
  {
    VLOG(1) << "Initialize bias ...";
    bias = buildBiasSimulator(trajectory);
    VLOG(1) << "done.";
  }
  catch (const std::bad_alloc& e)
  {
    LOG(FATAL) << "Could not create bias because number of samples is too high."
               << " Allocation failed: " << e.what();
  }

  VLOG(1) << "Initialize IMU ...";
  imu = std::make_shared<ze::ImuSimulator>(
           trajectory,
           bias,
           ze::RandomVectorSampler<3>::sigmas(
               ze::Vector3::Constant(FLAGS_simulation_imu_acc_noise_sigma)),
           ze::RandomVectorSampler<3>::sigmas(
               ze::Vector3::Constant(FLAGS_simulation_imu_gyr_noise_sigma)),
           FLAGS_simulation_imu_bandwidth_hz,
           FLAGS_simulation_imu_bandwidth_hz,
           FLAGS_simulation_imu_gravity_magnitude);
  VLOG(1) << "done.";

  return imu;
}

} // namespace event_camera_simulator
