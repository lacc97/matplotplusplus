//
// Created by Alan Freitas on 16/07/20.
//

#include <algorithm>
#include <matplot/axes_objects/circles.h>
#include <matplot/axes_objects/labels.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {
    circles::circles(class axes_type *parent, vector_proxy<double> x,
                     vector_proxy<double> y, vector_proxy<double> radius,
                     vector_proxy<double> start_angle,
                     vector_proxy<double> end_angle, vector_proxy<double> color)
        : axes_object(parent), x_(x.begin(), x.end()), y_(y.begin(), y.end()),
          radius_(radius.begin(), radius.end()),
          start_angle_(start_angle.begin(), start_angle.end()),
          end_angle_(end_angle.begin(), end_angle.end()),
          color_(color.begin(), color.end()) {}

    std::string circles::plot_string() {
        if (!user_face_color_ && color_.empty()) {
            // if user didn't set the color, get color from xlim
            face_color_ = parent_->get_color_and_bump();
            user_face_color_ = true;
        }
        std::string str = " '-' with circles linecolor ";
        str += color_.empty() ? "\"" + to_string(face_color_) + "\""
                              : " variable ";
        str += " linewidth " + num2str(line_width_) +
               " fillstyle solid border linecolor '" + to_string(line_color_) +
               "'";
        if (labels_) {
            str += ", " + labels_->plot_string();
        }
        return str;
    }

    std::string circles::legend_string(std::string_view title) {
        return " keyentry with circles linecolor var lw 4 fillstyle solid "
               "border linecolor 'black' title \"" +
               escape(title) + "\"";
    }

    std::string circles::data_string() {
        std::stringstream ss;
        for (size_t i = 0; i < x_.size(); ++i) {
            auto value_or_default = [](const std::vector<double> &v,
                                       size_t index, double default_value) {
                if (v.size() > index) {
                    return v[index];
                } else if (!v.empty()) {
                    return v[0];
                } else {
                    return default_value;
                }
            };
            if (color_.empty()) {
                ss << "    " << x_[i] << " " << y_[i] << " "
                   << value_or_default(radius_, i, 1) << " "
                   << value_or_default(start_angle_, i, 0) << " "
                   << value_or_default(end_angle_, i, 360) << "\n";
            } else {
                ss << "    " << x_[i] << " " << y_[i] << " "
                   << value_or_default(radius_, i, 1) << " "
                   << value_or_default(start_angle_, i, 0) << " "
                   << value_or_default(end_angle_, i, 360) << " "
                   << value_or_default(color_, i, 1) << "\n";
            }
        }
        ss << "e\n";
        if (labels_) {
            ss << labels_->data_string();
        }
        return ss.str();
    }

    bool circles::requires_colormap() { return true; }

    double circles::xmax() {
        auto it = std::max_element(x_.begin(), x_.end());
        if (it != x_.end()) {
            double this_max = *it + radius_[it - x_.begin()];
            if (labels_) {
                return std::max(this_max, labels_->xmax());
            } else {
                return this_max;
            }
        } else {
            return axes_object::xmax();
        }
    }

    double circles::xmin() {
        auto it = std::min_element(x_.begin(), x_.end());
        if (it != x_.end()) {
            double this_min = *it - radius_[it - x_.begin()];
            if (labels_) {
                return std::min(this_min, labels_->xmin());
            } else {
                return this_min;
            }
        } else {
            return axes_object::xmin();
        }
    }

    double circles::ymax() {
        auto it = std::max_element(y_.begin(), y_.end());
        if (it != y_.end()) {
            double this_max = *it + radius_[it - y_.begin()];
            if (labels_) {
                return std::max(this_max, labels_->ymax());
            } else {
                return this_max;
            }
        } else {
            return axes_object::ymax();
        }
    }

    double circles::ymin() {
        auto it = std::min_element(y_.begin(), y_.end());
        if (it != y_.end()) {
            double this_min = *it - radius_[it - y_.begin()];
            if (labels_) {
                return std::min(this_min, labels_->ymin());
            } else {
                return this_min;
            }
        } else {
            return axes_object::ymin();
        }
    }

    enum axes_object::axes_category circles::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    const std::vector<double> &circles::x() const { return x_; }

    class circles &circles::x(vector_proxy<double> x) {
        x_.assign(x.begin(), x.end());
        touch();
        return *this;
    }

    const std::vector<double> &circles::y() const { return y_; }

    class circles &circles::y(vector_proxy<double> y) {
        y_.assign(y.begin(), y.end());
        touch();
        return *this;
    }

    const std::vector<double> &circles::radius() const { return radius_; }

    class circles &circles::radius(vector_proxy<double> radius) {
        radius_.assign(radius.begin(), radius.end());
        touch();
        return *this;
    }

    const std::vector<double> &circles::start_angle() const {
        return start_angle_;
    }

    class circles &circles::start_angle(vector_proxy<double> start_angle) {
        start_angle_.assign(start_angle.begin(), start_angle.end());
        touch();
        return *this;
    }

    const std::vector<double> &circles::end_angle() const { return end_angle_; }

    class circles &circles::end_angle(vector_proxy<double> end_angle) {
        end_angle_.assign(end_angle.begin(), end_angle.end());
        touch();
        return *this;
    }

    const std::vector<double> &circles::color() const { return color_; }

    class circles &circles::color(vector_proxy<double> color) {
        color_.assign(color.begin(), color.end());
        touch();
        return *this;
    }

    const labels_handle &circles::labels() const { return labels_; }

    class circles &circles::labels(const labels_handle &labels) {
        labels_ = labels;
        touch();
        return *this;
    }

    bool circles::visible() const { return visible_; }

    class circles &circles::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    const color_array &circles::face_color() const { return face_color_; }

    class circles &circles::face_color(const color_array &face_color) {
        face_color_ = face_color;
        user_face_color_ = true;
        touch();
        return *this;
    }

    float circles::line_width() const { return line_width_; }

    class circles &circles::line_width(float line_width) {
        line_width_ = line_width;
        touch();
        return *this;
    }

    const color_array &circles::line_color() const { return line_color_; }

    class circles &circles::line_color(const color_array &line_color) {
        line_color_ = line_color;
        touch();
        return *this;
    }

} // namespace matplot
