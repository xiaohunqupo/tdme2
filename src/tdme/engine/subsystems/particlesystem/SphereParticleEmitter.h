#pragma once

#include <memory>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/primitives/Sphere.h>
#include <tdme/engine/subsystems/particlesystem/fwd-tdme.h>
#include <tdme/engine/subsystems/particlesystem/ParticleEmitter.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>

using std::unique_ptr;

using tdme::engine::primitives::Sphere;
using tdme::engine::subsystems::particlesystem::Particle;
using tdme::engine::subsystems::particlesystem::ParticleEmitter;
using tdme::engine::Color4;
using tdme::engine::Transform;
using tdme::math::Vector3;

/**
 * Sphere particle emitter
 * @author Andreas Drewke
 */
class tdme::engine::subsystems::particlesystem::SphereParticleEmitter final
	: public ParticleEmitter
{
private:
	int32_t count;
	int64_t lifeTime;
	int64_t lifeTimeRnd;
	float mass;
	float massRnd;
	unique_ptr<Sphere> sphere;
	unique_ptr<Sphere> worldSphere;
	Vector3 velocity;
	Vector3 velocityRnd;
	Color4 colorStart;
	Color4 colorEnd;

public:
	// forbid class copy
	FORBID_CLASS_COPY(SphereParticleEmitter)

	/**
	 * @param count number of particles to emit in one second
	 * @param lifeTime life time in milli seconds
	 * @param lifeTimeRnd life time rnd in milli seconds
	 * @param mass mass in kg
	 * @param massRnd mass rnd in kg
	 * @param sphere sphere
	 * @param velocity velocity in meter / seconds
	 * @param velocityRnd velocity rnd in meter / seconds
	 * @param colorStart color start
	 * @param colorEnd color end
	 */
	SphereParticleEmitter(int32_t count, int64_t lifeTime, int64_t lifeTimeRnd, float mass, float massRnd, Sphere* sphere, const Vector3& velocity, const Vector3& velocityRnd, const Color4& colorStart, const Color4& colorEnd);

	/**
	 * Destructor
	 */
	virtual ~SphereParticleEmitter();

	// overridden methods
	inline const Vector3& getCenter() const override {
		return worldSphere->getCenter();
	}

	inline int32_t getCount() const override {
		return count;
	}

	inline const Vector3& getVelocity() const {
		return velocity;
	}

	inline const Vector3& getVelocityRnd() const {
		return velocityRnd;
	}

	inline const Color4& getColorStart() const override {
		return colorStart;
	}

	inline void setColorStart(const Color4& colorStart) override {
		this->colorStart = colorStart;
	}

	inline const Color4& getColorEnd() const override {
		return colorEnd;
	}

	inline void setColorEnd(const Color4& colorEnd) override {
		this->colorEnd = colorEnd;
	}

	void emit(Particle* particle) override;
	void setTransform(const Transform& transform) override;

};
