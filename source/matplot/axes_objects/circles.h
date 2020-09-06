//
// Created by Alan Freitas on 16/07/20.
//

#ifndef MATPLOTPLUSPLUS_CIRCLES_H
#define MATPLOTPLUSPLUS_CIRCLES_H

#include <matplot/core/figure_type.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/axis_type.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

namespace matplot {
    class axes_type;
    class circles : public axes_object {
      public:
        explicit circles(class axes_type *parent);
        circles(class axes_type *parent, vector_proxy<double> x,
                vector_proxy<double> y, vector_proxy<double> radius = {},
                vector_proxy<double> start_angle = {},
                vector_proxy<double> end_angle = {},
                vector_proxy<double> color = {});

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        circles(const axes_handle &parent, Args&&... args)
            : circles(parent.get(), std::forward<Args>(args)...) {}

      public /* mandatory virtual functions */:
        // std::string set_variables_string() override;
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        // std::string unset_variables_string() override;
        bool requires_colormap() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        const std::vector<double> &x() const;

        class circles &x(vector_proxy<double> x);

        const std::vector<double> &y() const;

        class circles &y(vector_proxy<double> y);

        const std::vector<double> &radius() const;

        class circles &radius(vector_proxy<double> radius);

        const std::vector<double> &start_angle() const;

        class circles &start_angle(vector_proxy<double> start_angle);

        const std::vector<double> &end_angle() const;

        class circles &end_angle(vector_proxy<double> end_angle);

        const std::vector<double> &color() const;

        class circles &color(vector_proxy<double> color);

        const labels_handle &labels() const;

        class circles &labels(const labels_handle &labels);

        const color_array &face_color() const;

        class circles &face_color(const color_array &face_color);

        float line_width() const;

        class circles &line_width(float line_width);

        const color_array &line_color() const;

        class circles &line_color(const color_array &line_color);

        bool visible() const;

        class circles &visible(bool visible);

      public /* getters and setters bypassing the line_spec */:
      protected:
        void maybe_update_circles_color();

      protected:
        std::vector<double> x_{};           // x position
        std::vector<double> y_{};           // y position
        std::vector<double> radius_{};      // 1
        std::vector<double> start_angle_{}; // 0
        std::vector<double> end_angle_{};   // 360
        std::vector<double> color_{};       // colormap

        // Labels around circles
        labels_handle labels_;

        // Style
        color_array face_color_;
        bool user_face_color_{false};
        float line_width_{2.};
        color_array line_color_{0., 0., 0., 0.};

        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_CIRCLES_H
