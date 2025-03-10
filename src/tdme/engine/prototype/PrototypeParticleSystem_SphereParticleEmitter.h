#pragma once

#include <tdme/tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/prototype/fwd-tdme.h>
#include <tdme/engine/Color4.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>

using tdme::engine::Color4;
using tdme::math::Vector3;

/**
 * Prototype sphere particle emitter definition
 * @author Andreas Drewke
 */
class tdme::engine::prototype::PrototypeParticleSystem_SphereParticleEmitter final
{
private:
	int count;
	int64_t lifeTime;
	int64_t lifeTimeRnd;
	float mass;
	float massRnd;
	Vector3 velocity;
	Vector3 velocityRnd;
	Color4 colorStart;
	Color4 colorEnd;
	Vector3 center;
	float radius;

public:
	// forbid class copy
	FORBID_CLASS_COPY(PrototypeParticleSystem_SphereParticleEmitter)

	/**
	 * Public constructor
	 */
	PrototypeParticleSystem_SphereParticleEmitter();

	/**
	 * @returns count
	 */
	inline int getCount() {
		return count;
	}

	/**
	 * Set count
	 * @param count count
	 */
	inline void setCount(int count) {
		this->count = count;
	}

	/**
	 * @returns life time
	 */
	inline int64_t getLifeTime() {
		return lifeTime;
	}

	/**
	 * Set life time
	 * @param lifeTime life time
	 */
	inline void setLifeTime(int64_t lifeTime) {
		this->lifeTime = lifeTime;
	}

	/**
	 * @returns life time rnd
	 */
	inline int64_t getLifeTimeRnd() {
		return lifeTimeRnd;
	}

	/**
	 * Set life time rnd
	 * @param lifeTimeRnd life time rnd
	 */
	inline void setLifeTimeRnd(int64_t lifeTimeRnd) {
		this->lifeTimeRnd = lifeTimeRnd;
	}

	/**
	 * @returns mass
	 */
	inline float getMass() {
		return this->mass;
	}

	/**
	 * Set mass
	 * @param mass mass
	 */
	inline void setMass(float mass) {
		this->mass = mass;
	}

	/**
	 * @returns mass rnd
	 */
	inline float getMassRnd() {
		return massRnd;
	}

	/**
	 * Set mass rnd
	 * @param massRnd mass rnd
	 */
	inline void setMassRnd(float massRnd) {
		this->massRnd = massRnd;
	}

	/**
	 * @returns velocity
	 */
	inline const Vector3& getVelocity() {
		return velocity;
	}

	/**
	 * Set velocity
	 * @param velocity velocity
	 */
	inline void setVelocity(const Vector3& velocity) {
		this->velocity = velocity;
	}

	/**
	 * @returns velocity rnd
	 */
	inline const Vector3& getVelocityRnd() {
		return velocityRnd;
	}

	/**
	 * Set random velocity
	 * @param velocityRnd random velocity
	 */
	inline void setVelocityRnd(const Vector3& velocityRnd) {
		this->velocityRnd = velocityRnd;
	}

	/**
	 * @returns color start
	 */
	inline const Color4& getColorStart() {
		return colorStart;
	}

	/**
	 * Set start color
	 * @param colorStart start color
	 */
	inline void setColorStart(const Color4& colorStart) {
		this->colorStart = colorStart;
	}

	/**
	 * @returns color end
	 */
	inline const Color4& getColorEnd() {
		return colorEnd;
	}

	/**
	 * Set end color
	 * @param colorEnd color end
	 */
	inline void setColorEnd(const Color4& colorEnd) {
		this->colorEnd = colorEnd;
	}

	/**
	 * @returns center
	 */
	inline const Vector3& getCenter() {
		return center;
	}

	/**
	 * Set center
	 * @param center center
	 */
	inline void setCenter(const Vector3& center) {
		this->center = center;
	}

	/**
	 * @returns radius
	 */
	inline float getRadius() {
		return radius;
	}

	/**
	 * Set radius
	 * @param radius radius
	 */
	inline void setRadius(float radius) {
		this->radius = radius;
	}

};
