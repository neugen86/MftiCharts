import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import QtCharts 2.0

import MyTypes 1.0

Window {
    width: 1024
    height: 640

    visible: true

    title: qsTr("Chart")

    SeriesModel {
        id: series_model
    }

    PointGenerator {
        id: generator

        minimumX: 0
        maximumX: 600

        minimumY: 0
        maximumY: 400

        onGenerated: {
            series_model.append(point)
        }
    }

    RowLayout {
        anchors.fill: parent

        ChartView {
            id: chart_view

            Layout.fillWidth: true
            Layout.fillHeight: true

            legend.visible: false

            LineSeries {
                id: line_series

                XYPoint {
                    x: generator.minimumX
                    y: generator.minimumY
                }

                XYPoint {
                    x: generator.maximumX
                    y: generator.maximumY
                }
            }

            Item {
                id: handle_items

                anchors.fill: parent

                Repeater {
                    model: series_model

                    delegate: MovablePoint {
                        handle: model.handle
                        bound: chart_view.plotArea
                    }
                }
            }
        }

        ColumnLayout {
            CheckDelegate {
                text: qsTr("Interactive")
                checked: handle_items.visible

                onCheckedChanged: {
                    handle_items.visible = checked
                }
            }

            GroupBox {
                title: qsTr("Point Generation")

                RowLayout {
                    Button {
                        text: generator.paused ? qsTr("Resume") : qsTr("Pause")
                        onClicked: generator.toggleProcess()
                    }

                    Button {
                        text: qsTr("Stop")
                        enabled: generator.running

                        onClicked: {
                            generator.stop()
                            series_model.clear()
                        }
                    }
                }
            }

            PointListView {
                Layout.fillHeight: true

                series: series_model
            }
        }
    }

    Component.onCompleted: {
        series_model.setSeries(line_series)
    }
}
