class Point < ActiveRecord::Base
  belongs_to :bike_path

  def latlng
    [self.latitude, self.longitude]
  end
end
