class CreatePoints < ActiveRecord::Migration
  def change
    create_table :points do |t|
      t.float :latitude
      t.float :longitude
      t.references :bike_path, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
