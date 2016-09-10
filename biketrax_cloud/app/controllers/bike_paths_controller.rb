class BikePathsController < ApplicationController
  before_action :set_bike_path, only: [:show, :edit, :update, :destroy]

  # GET /bike_paths
  # GET /bike_paths.json
  def index
    @bike_paths = BikePath.all
  end

  # GET /bike_paths/1
  # GET /bike_paths/1.json
  def show
    respond_to do |format|
      format.html # show.html.erb
      format.json {
        render :json => @bike_path.to_json(:methods => [:polyline], :only => [:name])
      }
    end
  end

  def add_point
    point = Point.new
    point.longitude = params[:longitude]
    point.latitude = params[:latitude]
    point.bike_path = BikePath.first
    point.save

    render json: 'OK'
  end

  # GET /bike_paths/new
  def new
    @bike_path = BikePath.new
  end

  # GET /bike_paths/1/edit
  def edit
  end

  # POST /bike_paths
  # POST /bike_paths.json
  def create
    @bike_path = BikePath.new(bike_path_params)

    respond_to do |format|
      if @bike_path.save
        format.html { redirect_to @bike_path, notice: 'Bike path was successfully created.' }
        format.json { render :show, status: :created, location: @bike_path }
      else
        format.html { render :new }
        format.json { render json: @bike_path.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /bike_paths/1
  # PATCH/PUT /bike_paths/1.json
  def update
    respond_to do |format|
      if @bike_path.update(bike_path_params)
        format.html { redirect_to @bike_path, notice: 'Bike path was successfully updated.' }
        format.json { render :show, status: :ok, location: @bike_path }
      else
        format.html { render :edit }
        format.json { render json: @bike_path.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /bike_paths/1
  # DELETE /bike_paths/1.json
  def destroy
    @bike_path.destroy
    respond_to do |format|
      format.html { redirect_to bike_paths_url, notice: 'Bike path was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
  # Use callbacks to share common setup or constraints between actions.
  def set_bike_path
    @bike_path = BikePath.find(params[:id])
  end

  # Never trust parameters from the scary internet, only allow the white list through.
  def bike_path_params
    params.require(:bike_path).permit(:name, :datetime)
  end
end
