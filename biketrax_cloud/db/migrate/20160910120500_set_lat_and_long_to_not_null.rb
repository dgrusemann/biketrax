class SetLatAndLongToNotNull < ActiveRecord::Migration
  def change
    change_column :points, :latitude, :float, :null => false
    change_column :points, :longitude, :float, :null => false
  end
end
