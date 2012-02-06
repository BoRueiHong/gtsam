/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 *  @file  RangeFactor.H
 *  @author Frank Dellaert
 **/

#pragma once

#include <boost/lexical_cast.hpp>
#include <gtsam/geometry/concepts.h>
#include <gtsam/nonlinear/NonlinearFactor.h>

namespace gtsam {

	/**
	 * Binary factor for a range measurement
	 */
	template<class POSE, class POINT>
	class RangeFactor: public NonlinearFactor2<POSE, POINT> {
	private:

		double measured_; /** measurement */

		typedef RangeFactor<POSE, POINT> This;
		typedef NonlinearFactor2<POSE, POINT> Base;

		typedef typename POSE Pose;
		typedef typename POINT Point;

		// Concept requirements for this factor
		GTSAM_CONCEPT_RANGE_MEASUREMENT_TYPE(Pose, Point)

	public:

		RangeFactor() {} /* Default constructor */

		RangeFactor(const Symbol& poseKey, const Symbol& pointKey, double measured,
				const SharedNoiseModel& model) :
					Base(model, poseKey, PointKey), measured_(measured) {
		}

		virtual ~RangeFactor() {}

		/** h(x)-z */
		Vector evaluateError(const Pose& pose, const Point& point, boost::optional<Matrix&> H1, boost::optional<Matrix&> H2) const {
			double hx = pose.range(point, H1, H2);
			return Vector_(1, hx - measured_);
		}

		/** return the measured */
		double measured() const {
			return measured_;
		}

		/** equals specialized to this factor */
		virtual bool equals(const NonlinearFactor& expected, double tol=1e-9) const {
			const This *e = dynamic_cast<const This*> (&expected);
			return e != NULL && Base::equals(*e, tol) && fabs(this->measured_ - e->z_) < tol;
		}

		/** print contents */
		void print(const std::string& s="") const {
			Base::print(s + std::string(" range: ") + boost::lexical_cast<std::string>(measured_));
		}

	private:

		/** Serialization function */
		friend class boost::serialization::access;
		template<class ARCHIVE>
		void serialize(ARCHIVE & ar, const unsigned int version) {
			ar & boost::serialization::make_nvp("NonlinearFactor2",
					boost::serialization::base_object<Base>(*this));
			ar & BOOST_SERIALIZATION_NVP(measured_);
		}
	}; // RangeFactor

} // namespace gtsam
