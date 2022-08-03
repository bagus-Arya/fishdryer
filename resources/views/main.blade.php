@extends('layouts.mainlayout')
@section('container')

<div class="container py-3">
    <div class="row">
        <div class="col-md-12 col-lg-3">
            <div class="card card text-bg-light my-1">
                <div class="card-header">
                  Device Status
                </div>
                <div class="card-body">
                    <div class="row justify-content-center">
                        <div class="dot-device-status">
                            <span>ON</span>
                        </div>
                    </div>
                    <div class="row mt-2">
                        <i class="fa-solid fa-calendar-clock"></i>
                        <p class="card-text mb-0 mt-12"><i class="fa-solid fa-calendar-days"></i> Last Request : May 12,2022</p>
                        <p class="card-text mb-0"><i class="fa-solid fa-clock"></i> Time : 20:30</p>
                    </div>
                </div>
              </div>
        </div>
        <div class="col-md-12 col-lg-9">
            <div class="card card text-bg-light my-1">
                <div class="card-header">
                  Data Hari ini
                </div>
                <div class="card-body">
                  <div class="container-fluid">
                    <div class="row">
                        <p class="card-text mb-2"><i class="fa-solid fa-calendar-days"></i> Date : May 12,2022</p>
                    </div>
                    <div class="row justify-content-center">
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                <i class="fa-solid fa-temperature-high"></i>
                                  Suhu
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat blue">
                                            30 C
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                    <i class="fa-solid fa-weight-scale"></i>
                                  Berat
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat yellow">
                                            0.25 kg
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                    <i class="fa-solid fa-lightbulb"></i>
                                  Lampu
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat green">
                                            ON
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 d-lg-none">
                            
                        </div>
                    </div>
                    <div class="row">
                       <div class="col-12">
                        <div class="card my-1">
                            <div class="card-header">
                            <i class="fa-solid fa-chart-line"></i>
                              Graph Data
                            </div>
                            <div class="card-body">
                                <div class="row justify-content-center">
                                    {{-- <div>
                                        <canvas id="myChart"></canvas>
                                    </div> --}}
                                    <div class="col-xl-6">
                                        <div class="chart-hariini mt-2">
                                            <canvas id="myChart"></canvas>
                                        </div>
                                        
                                    </div>
                                    <div class="col-xl-6">
                                        <div class="chart-hariini mt-2">
                                            <canvas id="myChart2"></canvas>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                       </div>
                    </div>
                  </div>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-12">
            <div class="card card text-bg-light my-2">
                <div class="card-header">
                  Graph Range
                </div>
                <div class="card-body">
                    <form action="" method="get" class="row">
                        <div class="col-6 col-lg-3">
                            <label for="fromdate" class="form-label">From Date :</label>
                            <input type="date" class="form-control" id="fromdate" value="2011-09-29">
                        </div>
                        <div class="col-6 col-lg-3">
                            <label for="todate" class="form-label">To Date</label>
                            <input type="date" class="form-control" id="todate" value="2011-09-29">
                        </div>
                        <div class="col-12 col-lg-2 align-self-end">
                            <label for="sumbit" class="form-label d-none"></label>
                            <button type="submit" class="btn btn-success w-100 mb-lg-0 mb-3 mt-lg-0 mt-3 text-light">Filter</button>
                        </div>
                    </form>
                    <div class="row mt-2">
                        <div class="row justify-content-center">
                            <div class="col-xl-6">
                                <div class="chart-hariini mt-2">
                                    <canvas id="myChart3"></canvas>
                                </div>
                                
                            </div>
                            <div class="col-xl-6">
                                <div class="chart-hariini mt-2">
                                    <canvas id="myChart4"></canvas>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
              </div>
        </div>
    </div>
</div>

<div class="container">
    <footer class="d-flex flex-wrap justify-content-between align-items-center py-3 my-4 border-top">
      <div class="col-md-4 d-flex align-items-center">
        <img src="{{ asset('assests/image/fish.png')}}" class="footer-logo me-1" alt="...">
        <span class="mb-3 mb-md-0 text-muted"> © 2022 Fish Dryer, Inc</span>
      </div>
  
    </footer>
</div>

<script>
    const labels = [
      'January',
      'February',
      'March',
      'April',
      'May',
      'June',
    ];
  
    const data = {
      labels: labels,
      datasets: [{
        label: 'My First dataset',
        backgroundColor: 'rgb(255, 99, 132)',
        borderColor: 'rgb(255, 99, 132)',
        data: [0, 10, 5, 2, 20, 30, 45],
      }]
    };
  
    const config = {
      type: 'line',
      data: data,
      options: {
        responsive:true,
        maintainAspectRatio:false
      }
    };
</script>
<script>
    const myChart = new Chart(
      document.getElementById('myChart'),
      config
    );
    const myChart2 = new Chart(
      document.getElementById('myChart2'),
      config
    );
    const myChart3 = new Chart(
      document.getElementById('myChart3'),
      config
    );
    const myChart4 = new Chart(
      document.getElementById('myChart4'),
      config
    );
</script>
@endsection