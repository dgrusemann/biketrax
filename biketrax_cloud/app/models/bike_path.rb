class BikePath < ActiveRecord::Base
  has_many :points

  def polyline_points
    self.points.map(&:latlng)
  end

  def polyline
    Polylines::Encoder.encode_points(self.polyline_points)
  end
end
