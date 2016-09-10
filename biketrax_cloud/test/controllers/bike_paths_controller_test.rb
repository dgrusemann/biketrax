require 'test_helper'

class BikePathsControllerTest < ActionController::TestCase
  setup do
    @bike_path = bike_paths(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:bike_paths)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create bike_path" do
    assert_difference('BikePath.count') do
      post :create, bike_path: { datetime: @bike_path.datetime, name: @bike_path.name }
    end

    assert_redirected_to bike_path_path(assigns(:bike_path))
  end

  test "should show bike_path" do
    get :show, id: @bike_path
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @bike_path
    assert_response :success
  end

  test "should update bike_path" do
    patch :update, id: @bike_path, bike_path: { datetime: @bike_path.datetime, name: @bike_path.name }
    assert_redirected_to bike_path_path(assigns(:bike_path))
  end

  test "should destroy bike_path" do
    assert_difference('BikePath.count', -1) do
      delete :destroy, id: @bike_path
    end

    assert_redirected_to bike_paths_path
  end
end
