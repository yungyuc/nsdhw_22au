# BEV road map Generator 
## Basic Information

A library to generate a BEV semantic map from scene attributes

* github repository: [BEV-ROAD-MAP-Generator](https://github.com/chiehchiYang/BEV-ROAD-MAP-Generator)

## Problem To Solve

As the autonomous cars and other traffic agents move on the road, most of the time the reasoning does not need to account for height, making **Birds-Eye-View (BEV)** a sufficient representation.
And there are many methods to create a bev road map. Here I will focus on how to generate a rough bev map from scene attribtues (See below figure).
Given a list of scene attributes, we can generate a bird eye view map with semantic labels(road, cross walk, lane mark ... )

![](https://i.imgur.com/8UuxkcQ.png )


### Here is the example from the paper ( ref. 1)
![](https://i.imgur.com/dPsNXAW.png)


## Goal 

1. Implement a c++ version of rendering semantic map in BEV
2. Support map scaling with different size
3. Use pybind to make it easily to call in python
4. Map rendering engine
5. Generate a route on the map ( A* search algorithm)



Here is the pseudo code from the parper ( Ref. 1)
![](https://i.imgur.com/P0N7k6v.png)


## Prospective Users

People who want to draw semantic road map with some parameters.

## System Architecture
![](https://i.imgur.com/xb4VAsm.png)
## API Description
**I will make a  assumption that the camera/ego car is located at the bottom center**
### Python

1. BEV_map( parameters... )
2. Route_generate( high_level_command_list )
3. Show()
4. Transfer_to_numpy_array()

#### High_level_commands:
- Turn righ
- Turn left
- Go straight
- Merge left
- Merge right

### C++

1. Empty_map( scale_value_meter_to_pixel, height_of_map, width_of_map)
2. Draw_main_lane( left_lane_num, right_lane_num, lane_width ) 
3. Draw_side_walk( some constrain )
4. Draw_crosswalk( some constrain )
5. Create_intersection( distance_to_ego_car, road_curve_angle )
6. Create_T_junction( distance_to_ego_car, angle_to_main_road  )
7. Generate_a_route( high_level_command_list )
8. scale_map( scale_value_meter_to_pixel )

## Engineering Infrastructure

|Category|Tools|
|--|--|
|Build System|Make|
|Testing Framework|pytest|
|Version Control|Git|

## Schedule


### Week 1 (10/31): 

- Setup the environment


### Week 2 (11/7):
- Create empty map (Grid) and draw straight lane
- Implement Scale_map function


### Week 3 (11/14):

- Implement Draw_side_walk and  Draw_crosswalk
- Write a python test for a simple cell rule.

### Week 4 (11/21):

- Implement Create_intersection and  Create_T_junction



### Week 5 (11/28):

- Implement Generate_a_route and A* search algorith with different high level commands


### Week 6 (12/5):

- Finish up, debug, and write the documentation

### Week 7 (12/12):

- Make slides and prepare for the presentation

### Week 8 (12/19):

- Prepare the presentation


## References

* Weakly But Deeply Supervised Occlusion-Reasoned Parametric Road Layouts: https://arxiv.org/pdf/2104.06730.pdf
