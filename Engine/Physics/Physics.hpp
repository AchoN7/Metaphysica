#pragma once

class Physics {
public:

	Physics() : m_physicsOn(false), m_timeFactor(1) {}

	void pause() { if (m_physicsOn) m_physicsOn = false; }
	void resume() { if (!m_physicsOn) m_physicsOn = true; }
	void speedup() { m_timeFactor = 10; }
	void normal() { m_timeFactor = 1; }

private:

	bool m_physicsOn;
	unsigned int m_timeFactor;

};