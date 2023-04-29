#pragma once

class Physics {
public:

	Physics() : m_physicsOn(false) {}

	//for now, only sphere collisions with the planet
	void updateStar(Star& star, float deltaTime) {
		if (!m_physicsOn) return;

		float timeFactor = 100;
		glm::vec3 newOffset = glm::rotate(star.getOffsetFromCamera(), glm::radians(deltaTime / 100), star.getAxisOfRotation());
		star.setOffsetFromCamera(newOffset);
	}

	void pause() { if (m_physicsOn) m_physicsOn = false; }
	void resume() { if (!m_physicsOn) m_physicsOn = true; }

private:

	bool m_physicsOn;

};