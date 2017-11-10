#include "../src/Network.hpp"
#include "../src/Neuron.hpp"
#include "../src/Current.hpp"
#include "../src/Constants.hpp"
#include <cmath>
#include "googletest/include/gtest/gtest.h"

TEST(CurrentTest, CorrectOnOffTest) { 
	// test correct values of current
	Current c(1.0, 100, 200);
    EXPECT_EQ(c.getValue(99), 0.0);
    EXPECT_EQ(c.getValue(100), 1.0);
    EXPECT_EQ(c.getValue(110), 1.0);
    EXPECT_EQ(c.getValue(201), 0.0);
}

TEST(NeuronReactionTest, PositiveInput) {
	if (!C::IS_BACKGROUND_NOISE) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		// test one update
		n.update(1, 1.0);
		EXPECT_EQ(C::MEMBRANE_RESISTANCE * (1.0 - exp(- C::STEP_DURATION / C::TAU)), n.getPotential());
	}
}

TEST(NeuronReactionTest, NegativeInput) {
	if (!C::IS_BACKGROUND_NOISE) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		// test one update
		n.update(1, -1.0);
		EXPECT_EQ(-1 * C::MEMBRANE_RESISTANCE * (1.0 - exp(- C::STEP_DURATION / C::TAU)), n.getPotential());
	}
}

TEST(NeuronSpikesTest, TimeTillFirstSpike) {
	if (!C::IS_BACKGROUND_NOISE) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		long count = 0;
		double I_ext = 1.01;
		do {
			n.update(1, I_ext);
			++count;
		} while (!n.isRefractory() && count < 3000); // set additional limit to prevent infinite loops 
		
		// the neuron should be refractory at 92.5 ms, one step after the first spike
		EXPECT_EQ(n.getClock(), 925);
	}
}

TEST(NeuronSpikesTest, CorrectSpikes) {
	if (!C::IS_BACKGROUND_NOISE) {
		Neuron n(true, C::TAU, C::MEMBRANE_RESISTANCE);
		
		double I_ext = 1.01;
		
		// for an I_ext of 1.01, the spike times should be at
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

TEST(NeuronSpikesTest, CorrectSpikeReception) {
	if (!C::IS_BACKGROUND_NOISE) {
		Neuron n = Neuron();

		// transmit a spike at start
		n.receive(C::J_EXCITATORY, C::TRANSMISSION_DELAY);

		// update the neuron so it's just over the delay
		n.update(C::TRANSMISSION_DELAY + 1, 0);

		// the neuron's membrane potential should be equal
		// to the transmitted potential
		EXPECT_EQ(n.getPotential(), C::J_EXCITATORY);
	}
}

TEST(TwoNeuronsTest, CorrectBehaviour) {
	Neuron* n1 = new Neuron();
	Neuron* n2 = new Neuron();
	
	n1->addConnectionTarget(1);
	
	std::vector<Neuron*> neurons = {};
	neurons.push_back(n1);
	neurons.push_back(n2);
	
	long t = 0;
	long stop = 10000;
	while (t < stop) {
		
		for (auto neuron : neurons) {
			
			bool spiked = neuron->update(1, 0.0);
			
			if (spiked) {				
				for (auto idx : neuron->getConnectionTargets()) {
					neurons[idx]->receive(neuron->getTransmissionValue(), t + C::TRANSMISSION_DELAY);
				}
			}
		}
		
		++t;
	}
	
	// theoretical maximum is 500 spikes
	EXPECT_TRUE(neurons[0]->getNbSpikes() < stop / C::REFRACTORY_TIME);
	EXPECT_TRUE(neurons[1]->getNbSpikes() < stop / C::REFRACTORY_TIME);
}
 
TEST(NeuronSpikesTest, NoSpikesOnInit) { 
	Neuron n = Neuron();
	
	// neuron should not be refractory just after instanciation
    EXPECT_FALSE(n.isRefractory());

    // neuron should start without any emitted spikes
    EXPECT_TRUE(n.getNbSpikes() == 0);
}


int main(int argc, char**argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
