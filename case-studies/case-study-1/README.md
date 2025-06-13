# Mega Music Festival Shuttle Network Simulation

## Scenario
The government has approved a three-day mega music festival on the outskirts of the city. Tens of thousands of attendees will be arriving from different railway stations and airport terminals. To reduce the carbon footprint, private vehicles are banned. Therefore, the event organizers must rely on shuttle buses to transport people to the festival grounds.

As part of the festival logistics team, your task is to model and analyze the existing shuttle routes and optimize the transportation network under the following constraints:

- Limited bus capacity on certain roads.
- Traffic bottlenecks.
- Route availability and restrictions (e.g., some routes not usable after dark).

---

## Task List

### 1. Map the Shuttle Network
- Define all railway stations and airport terminals as entry points.
- Define the venue as the destination.
- Identify all intermediate city junctions, streets, overpasses, and traffic signals.
- Assign maximum people capacity (per hour) for each road.
- Mark which routes are active/inactive at different times of day.

### 2. Simulate the Movement
- Use a small data set for initial testing.
- Simulate attendee movement from each entry point to the venue.
- Test system behavior under road congestion and use of alternate routes.
- Identify network breakdowns and points of inefficiency.

### 3. Iterate the Network
- Add or remove shuttles on routes and observe outcomes.
- Change bus capacities or redirect through less congested junctions.
- Track how many people are transported to the venue in one hour.

---

## Design Considerations and Challenges
- **Prioritization**: How to prioritize stations with more incoming trains/flights.
- **Time Restrictions**: Some routes cannot be used after dark.
- **Shuttle Limits**: Pollution laws restrict the number of shuttles per road.
- **Inefficiencies**: Adding more shuttles to already busy roads may not help.

### Constraints
- Capacity-limited movement.
- Conservation of people: Transfers between shuttles must be accounted for.
- Alternate/multiple paths needed for high throughput.
- Bottleneck roads can cap overall efficiency.
- More resources do not always mean higher output.

---

## Reflection Questions

### Level 1:
- What are the entry and destination points in your transport map?
- Which roads handled the most number of shuttles?

### Level 2:
- What criteria did you use to decide which routes should carry more shuttles?
- How did you manage shared routes for multiple entry points?

### Level 3:
- Which routes in your model limited the total people reaching the venue?
- What shuttle movement patterns emerged when alternate routes were used?
- What happened when traffic was diverted from a busy to a lesser-used road?

### Level 4:
- If the crowd size doubles on the last day, how should the network adapt?
- Could your model support emergency evacuation planning (e.g., natural disasters)? Why or why not?
- What policy changes (e.g., overnight shuttle operations, lifting road restrictions) could significantly improve efficiency?

---

## End Goal
Prepare:
- Worksheets for planning and recording simulation data.
- Route network designs.
- Working simulation code.
- A gamified version of the system for interactive learning and engagement.

