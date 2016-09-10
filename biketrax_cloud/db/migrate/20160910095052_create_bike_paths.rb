class CreateBikePaths < ActiveRecord::Migration
  def change
    create_table :bike_paths do |t|
      t.string :name
      t.datetime :datetime

      t.timestamps null: false
    end
  end
end
