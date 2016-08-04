/*global angular*/
/*global $*/

var app = angular.module('IndexApp', []);

app.controller('SystemStateController', function($scope, $http) {
  $scope.refresh = function() {
    $('.text').addClass('text_init');
    $http({
      method: 'GET',
      url: './sys/stat'
    }).then(function (result) {
      //console.dir(result);
      $scope.stat = result.data;
      $('.text').removeClass('text_init');
    }, function (err) {
      console.error(err);
    });
  };
});