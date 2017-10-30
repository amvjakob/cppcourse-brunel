#include "Network.hpp"
#include "Neuron.hpp"
#include "Current.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>
#include "googletest/include/gtest/gtest.h"

constexpr bool NO_EXTERNAL_INPUT = false;

TEST(Current, CorrectOnOffTest) { 
	// test correct values of current
	Current c(1.0, 100, 200);
    EXPECT_EQ(c.getValue(99), 0.0);
    EXPECT_EQ(c.getValue(100), 1.0);
    EXPECT_EQ(c.getValue(110), 1.0);
    EXPECT_EQ(c.getValue(201), 0.0);
}

// NOTE: the following 4 tests need no external input 
TEST(NeuronTest, PositiveInput) {
	if (NO_EXTERNAL_INPUT) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		// test one update
		n.update(1, 1.0);
		EXPECT_EQ(C::MEMBRANE_RESISTANCE * (1.0 - exp(- C::STEP_DURATION / C::TAU)), n.getPotential());
	}
}

TEST(NeuronTest, NegativeInput) {
	if (NO_EXTERNAL_INPUT) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		// test one update
		n.update(1, -1.0);
		EXPECT_EQ(-1 * C::MEMBRANE_RESISTANCE * (1.0 - exp(- C::STEP_DURATION / C::TAU)), n.getPotential());
	}
}

TEST(NeuronTest, TimeTillFirstSpike) {
	if (NO_EXTERNAL_INPUT) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		long count = 0;
		double I_ext = 1.01;
		do {
			n.update(1, I_ext);
			++count;
		} while (!n.isRefractory() && count < 3000);
		
		std::cout << "Count: " << count << std::endl;
		
		EXPECT_TRUE(count > 1);
	}
}

TEST(NeuronTest, CorrectSpikes) {
	if (NO_EXTERNAL_INPUT) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		double I_ext = 1.01;
		
		// for an I_ext of 1.01, the spike times should be 
		// t=92.4ms, t=186.8ms and at t=281.2ms
		for (int spikeTime : { 924, 1868, 2812 }) {
			// update neuron to the step before the spike
			n.update(spikeTime - (int) n.getClock(), I_ext);
			EXPECT_FALSE(n.isRefractory());
			
			// update the neuron 1 step to trigger a spike
			n.update(1, I_ext);
			EXPECT_TRUE(n.isRefractory());
		}
		
	}
}
 
TEST(NeuronTest, NoSpikesTest) { 
	Neuron n = Neuron();
	
	// neuron should not be refractory just after instanciation
    ASSERT_FALSE(n.isRefractory());
}

int main(int argc, char**argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
